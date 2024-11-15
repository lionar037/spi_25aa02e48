#include <iostream>
#include <fstream>

int main() {
    // Define the text of the GNU General Public License
    std::string license_text = R"(
             GNU GENERAL PUBLIC LICENSE
                Version 1, 15 Noviembre 2024

    Copyright (C) <year>  <name of author>
    Everyone is permitted to copy and distribute verbatim copies
    of this license document, but changing it is not allowed.

    0. This License applies to any program or other work which contains
       a notice placed by the copyright holder saying it may be distributed
       under the terms of this General Public License. The "Program", below,
       refers to any such program or work, and a "work based on the Program"
       means either the Program or any derivative work under copyright law:
       that is to say, a work containing the Program or a portion of it,
       either verbatim or with modifications, that is so combined with other
       works as to form a larger program.

    1. You may copy, modify, distribute and sublicense the Program, under
       the terms of this License. You may do so in any medium, under the
       following conditions:
       a. You must provide a copy of this license in each source code
          distribution.
       b. If you distribute a binary or executable version of the Program,
          you must make the source code available.
       c. You may not impose any further restrictions on the rights granted
          by this License.
          
    2. You are not required to accept this License, since you have not
       signed it. However, nothing else grants you permission to modify
       or distribute the Program. These actions are prohibited by law if
       you do not accept this License.

    3. If you distribute copies of the Program, whether gratis or for a
       fee, you must pass on the full text of this license along with it.
       You must also keep intact all notices stating that this Program is
       released under the GNU General Public License. Any modified versions
       of the Program must also be distributed under the same license.

    4. You may not use the name of the Free Software Foundation or the name
       of any of its contributors to promote or endorse any modified version
       of this Program without specific prior written permission.

    5. This program is distributed "as is" without any warranty. See the
       full warranty disclaimer in the License.

    END OF TERMS AND CONDITIONS

    How to Apply These Terms to Your New Programs

    If you develop a new program and you want it to be of the greatest
    benefit to the public, the best way to do this is to make it free software
    which everyone can redistribute and change under these terms.
)";

    // Specify the name of the LICENSE file
    std::ofstream license_file("LICENSE");

    // Check if the file opened successfully
    if (license_file.is_open()) {
        // Write the license text to the file
        license_file << license_text;
        std::cout << "LICENSE file created successfully." << std::endl;
    } else {
        std::cerr << "Error opening the LICENSE file." << std::endl;
    }

    // Close the file
    license_file.close();

    return 0;
}
