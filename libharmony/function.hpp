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

#ifndef LIBHARMONY_FUNCTION_HPP
#define LIBHARMONY_FUNCTION_HPP

#include <libharmony/action.hpp>

#include <string>
#include <vector>

namespace harmony {

class Function {
public:
    std::string m_strName;
    std::vector< Action > m_vecActions;
    std::string toString() const;
};

std::string Function::toString() const {
    std::string ret = "    Function: ";
    ret.append(m_strName);
    ret.append("\n      Commands:");
    std::vector<Action>::const_iterator it = m_vecActions.begin();
    std::vector<Action>::const_iterator ite = m_vecActions.end();
    for(; it != ite; ++it) {
        ret.append("\n\t");
        ret.append(it->toString());
    }
    ret.append("\n");
    return ret;
}

} /* namespace libharmony */

#endif // LIBHARMONY_FUNCTION_HPP
