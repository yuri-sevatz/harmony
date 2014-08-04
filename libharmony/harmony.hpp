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

#ifndef LIBHARMONY_HARMONY_HPP
#define LIBHARMONY_HARMONY_HPP

#include <libharmony/action.hpp>
#include <libharmony/device.hpp>
#include <libharmony/function.hpp>

// TODO: Hide me behind d_ptr
#include <libharmony/csocket.hpp>

#include <map>
#include <string>
#include <vector>

namespace harmony {
    
extern std::string errorString;
extern std::string resultString;

void log(const char* message, bool bQuiet);

const std::string ReadAuthorizationTokenFile();
int WriteAuthorizationTokenFile(const std::string& strAuthorizationToken);

int parseConfiguration(const std::string& strConfiguration, std::map< std::string, std::string >& mapActivities, std::vector< Device >& vecDevices);

int parseControlGroup(const std::string& strControlGroup, std::vector<Function>& vecDeviceFunctions, const std::string& strDeviceID);

int parseFunction(const std::string& strFunction, std::vector<Function>& vecDeviceFunctions, const std::string& strDeviceID);

int parseAction(const std::string& strAction, std::vector<Action>& vecDeviceActions, const std::string& strDeviceID);

int submitCommand(csocket* commandcsocket, std::string& strAuthorizationToken, std::string strCommand, std::string strCommandParameterPrimary, std::string strCommandParameterSecondary);

int swapAuthorizationToken(csocket* authorizationcsocket, std::string& strAuthorizationToken);

int connectToHarmony(std::string strHarmonyIPAddress, int harmonyPortNumber, csocket& harmonyCommunicationcsocket);

int startCommunication(csocket* communicationcsocket, std::string strUserName, std::string strPassword);

int harmonyWebServiceLogin(std::string strUserEmail, std::string strPassword, std::string& strAuthorizationToken);

std::string base64_decode(std::string const& encoded_string);

std::string base64_encode(char const* bytes_to_encode, unsigned int in_len);

} /* namespace harmony */

#endif // LIBHARMONY_HARMONY_HPP
