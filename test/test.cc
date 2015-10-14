#include <bt/bt.h>
#include <iostream>
#include <string>
#include <algorithm>


int main(int argc, char **argv)
{

 try
 {
  ipr::bt con("66:35:56:78:90:AB",'>'); 
  
  std::cout << "Write end to exit:\n";
  std::string out,in;
    while (in != "end\r\n")
    {
        std::cout << "Message: ";
        std::cin >> in;
        in.push_back('\r');
        con.write(in);
        con.read(out);
        std::cout << "Echo: " << out << "\n";
        out.clear();
    }
  
 }
 catch(const std::exception &e)
 {
  std::cout << e.what() << " " << argv[1] << "\n";
  return -1;
 }

 return 0;
}

