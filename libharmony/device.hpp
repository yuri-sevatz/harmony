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

#ifndef LIBHARMONY_DEVICE_HPP
#define LIBHARMONY_DEVICE_HPP

#include <libharmony/function.hpp>

#include <string>
#include <vector>

namespace harmony {

class Device {
public:
    std::string toString() const;

    std::string m_strID;
    std::string m_strLabel;
    std::string m_strManufacturer;
    std::string m_strModel;
    std::string m_strType;
    std::vector< Function > m_vecFunctions;
};

inline std::string Device::toString() const {
    std::string ret = m_strType;
    ret.append(": ");
    ret.append(m_strLabel);
    ret.append(" (ID = ");
    ret.append(m_strID);
    ret.append(")\n");
    ret.append(m_strManufacturer);
    ret.append(" - ");
    ret.append(m_strModel);
    ret.append("\nFunctions: \n");
    std::vector<Function>::const_iterator it = m_vecFunctions.begin();
    std::vector<Function>::const_iterator ite = m_vecFunctions.end();
    for(; it != ite; ++it) {
        ret.append(it->toString());
    }
    return ret;
}

} /* namespace harmony */

#endif // LIBHARMONY_DEVICE_HPP
