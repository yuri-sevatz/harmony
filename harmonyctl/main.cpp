/******************************************************************************
  Permission is hereby granted, free of charge, to any person obtaining a copy  
  of this software and associated documentation files (the "Software"), to deal 
  in the Software without restriction, including without limitation the rights 
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
  copies of the Software, and to permit persons to whom the Software is 
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
  SOFTWARE.
*/

#include <libharmony/harmony.hpp>

// TODO: Maybe hide me?
#include <libharmony/include/config.h>

#include <iostream>
#include <cstdlib>

#include <string>

using namespace harmony;
using namespace std;

void help(const std::string & app) {
    cout << "Logitech Harmony Control" << endl;
    cout << "Usage: " << app <<
        "[email] [password] [harmony_ip] [command (optional)]"
    << endl;
    cout << "\tCommands:" << endl;
    cout << "\t\tlist_activities" << endl;
    cout << "\t\tlist_activities_raw" << endl;
    cout << "\t\tget_current_activity_id" << endl;
    cout << "\t\tget_current_activity_id_raw" << endl;
    cout << "\t\tstart_activity [ID]" << endl;
    cout << "\t\tlist_devices" << endl;
    cout << "\t\tlist_devices_raw" << endl;
    cout << "\t\tlist_commands" << endl;
    cout << "\t\tlist_device_commands [deviceId]" << endl;
    cout << "\t\tlist_device_commands_raw [deviceId]" << endl;
    cout << "\t\tissue_device_command [deviceId] [command]" << endl;
    cout << "\t\tget_config" << endl;
    cout << "\t\tget_config_raw" << endl;
}

int main(int argc, char * argv[]) {
    if (argc < 4) {
        help(argc ? argv[0] : "");
        return 0;
    }

    const string strUserEmail = argv[1];
    const string strUserPassword = argv[2];
    const string strHarmonyIP = argv[3];
    
	string strCommand;
    string strCommandParameterPrimary;
    string strCommandParameterSecondary;
    
    int harmonyPortNumber = HARMONY_COMMUNICATION_PORT;

    // User requested an action to be performed
    if(argc >= 5) {
        strCommand = argv[4];
    }
    if(argc>=6) {
        strCommandParameterPrimary = argv[5];
    }

    if(argc==7) {
        strCommandParameterSecondary = argv[6];
    }

    bool bQuietMode = false;

    if(strCommand.length() && strCommand.find("_raw") != string::npos) {
        bQuietMode = true;
    }

    // Read the token
    string strAuthorizationToken = ReadAuthorizationTokenFile();

    //printf("\nLogin Authorization Token is: %s\n\n", strAuthorizationToken.c_str());

    bool bAuthorizationComplete = false;

    if(strAuthorizationToken.length() > 0)
    {
        csocket authorizationcsocket;
        if(connectToHarmony(strHarmonyIP, harmonyPortNumber, authorizationcsocket) == 1)
        {
            log("HARMONY COMMUNICATION LOGIN    : FAILURE", false);
            printf("ERROR : %s\n", errorString.c_str());
            return 1;
        }

        if(swapAuthorizationToken(&authorizationcsocket, strAuthorizationToken) == 0)
        {
            // Authorization Token found in the file worked.  
            // Bypass authorization through Logitech's servers.
            log("LOGITECH WEB SERVICE LOGIN     : BYPASSED", bQuietMode);

            bAuthorizationComplete = true;
        }
        
    }

    
    if(bAuthorizationComplete == false)
    {
        // Log into the Logitech Web Service to retrieve the login authorization token
        if(harmonyWebServiceLogin(strUserEmail, strUserPassword, strAuthorizationToken) == 1)
        {
            log("LOGITECH WEB SERVICE LOGIN     : FAILURE", false);
            printf("ERROR : %s\n", errorString.c_str());
            return 1;
        }
        log("LOGITECH WEB SERVICE LOGIN     : SUCCESS", bQuietMode);

        //printf("\nLogin Authorization Token is: %s\n\n", strAuthorizationToken.c_str());

        // Write the Authorization Token to an Authorization Token file to bypass this step
        // on future sessions
        WriteAuthorizationTokenFile(strAuthorizationToken);

        // Log into the harmony hub to convert the login authorization token for a 
        // session authorization token
    
        csocket authorizationcsocket;
        if(connectToHarmony(strHarmonyIP, harmonyPortNumber, authorizationcsocket) == 1)
        {
            log("HARMONY COMMUNICATION LOGIN    : FAILURE", false);
            printf("ERROR : %s\n", errorString.c_str());
            return 1;
        }


        if(swapAuthorizationToken(&authorizationcsocket, strAuthorizationToken) == 1)
        {
            log("HARMONY COMMUNICATION LOGIN    : FAILURE", false);
            printf("ERROR : %s\n", errorString.c_str());
            return 1;
        }
    }

    log("HARMONY COMMUNICATION LOGIN    : SUCCESS", bQuietMode);


    //printf("\nSession Authorization Token is: %s\n\n", strAuthorizationToken.c_str());

    // We've successfully obtained our session authorization token from the harmony hub 
    // using the login authorization token we received earlier from the Logitech web service.
    // Now, disconnect from the harmony and reconnect using the mangled session token 
    // as our username and password to issue a command.


    csocket commandcsocket;
    if(connectToHarmony(strHarmonyIP, harmonyPortNumber, commandcsocket) == 1)
    {
        log("HARMONY COMMAND SUBMISSION     : FAILURE", false);
        printf("ERROR : %s\n", errorString.c_str());
        return 1;
    }

    string strUserName = strAuthorizationToken;
    //strUserName.append("@connect.logitech.com/gatorade.");
    string strPassword = strAuthorizationToken;
    
    if(startCommunication(&commandcsocket, strUserName, strPassword) == 1)
    {
        errorString = "Communication failure";
        return 1;
    }

    string lstrCommand = strCommand;

    if(strCommand == "list_activities"              || 
        strCommand == "list_activities_raw"         || 
        strCommand == "list_devices"                || 
        strCommand == "list_devices_raw"            || 
        strCommand == "list_commands"               || 
        strCommand == "list_device_commands"        || 
        strCommand == "list_device_commands_raw"    || 
        strCommand == "get_config")
    {
        lstrCommand = "get_config_raw";
    }

    if(submitCommand(&commandcsocket, strAuthorizationToken, lstrCommand, strCommandParameterPrimary, strCommandParameterSecondary) == 1)
    {
        log("HARMONY COMMAND SUBMISSION     : FAILURE", false);
        printf("ERROR : %s\n", errorString.c_str());
        return 1;
    }

    log("HARMONY COMMAND SUBMISSION     : SUCCESS", bQuietMode);
    
    if(lstrCommand == "get_config_raw")
    {
        map< string, string> mapActivities;
        vector< Device > vecDevices;
        if(parseConfiguration(resultString, mapActivities, vecDevices) == 1)
        {
            log("PARSE ACTIVITIES AND DEVICES   : FAILURE", false);
            printf("ERROR : %s\n", errorString.c_str());
            return 1;
        }

        if(strCommand == "list_activities" || strCommand == "list_activities_raw" )
        {
            resultString = "";

            if(strCommand == "list_activities")
            {
                resultString = "Activities Available via Harmony : \n\n";
            }
        
            map< string, string>::iterator it = mapActivities.begin();
            map< string, string>::iterator ite = mapActivities.end();
            for(; it != ite; ++it)
            {
                resultString.append(it->first);
                resultString.append(" - ");
                resultString.append(it->second);
                resultString.append("\n");

            }
        }

        if( strCommand == "list_devices" || strCommand == "list_devices_raw" )
        {
            resultString = "";
            
            if( strCommand == "list_devices" )
            {
                resultString = "Devices Controllable via Harmony : \n\n";
            }

            vector< Device >::iterator it = vecDevices.begin();
            vector< Device >::iterator ite = vecDevices.end();
            for(; it != ite; ++it)
            {
                resultString.append(it->m_strID );
                resultString.append(" - ");
                resultString.append(it->m_strLabel );
                resultString.append("\n");
                
            }
        }

        if(strCommand == "list_commands" || strCommand == "list_commands_raw" )
        {
            resultString = "";
            
            if(strCommand == "list_commands")
            {
                resultString = "Devices Controllable via Harmony with Commands : \n\n";
            }
            vector< Device >::iterator it = vecDevices.begin();
            vector< Device >::iterator ite = vecDevices.end();
            for(; it != ite; ++it)
            {
                resultString.append(it->toString());
                resultString.append("\n\n\n");
            }
        }

        if(strCommand == "list_device_commands" || strCommand == "list_device_commands_raw")
        {
            resultString = "";
            
            if(strCommand == "list_device_commands")
            {
                resultString = "Harmony Commands for Device: \n\n";
            }
            
            vector< Device >::iterator it = vecDevices.begin();
            vector< Device >::iterator ite = vecDevices.end();
            for(; it != ite; ++it)
            {
                if(it->m_strID == strCommandParameterPrimary)
                {
                    if(strCommandParameterSecondary.length())
                    {
                        if(strCommandParameterSecondary == it->m_strID)
                        {
                            resultString.append(it->toString());
                            resultString.append("\n\n\n");
                        }
                    }
                    else
                    {
                        resultString.append(it->toString());
                        resultString.append("\n\n\n");
                    }
                    

                    break;
                }
            }
        }
        log("PARSE ACTIVITIES AND DEVICES   : SUCCESS", bQuietMode);
    }

    printf("%s\n\n", resultString.c_str());

    return 0;
}

