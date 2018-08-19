#include <iostream>
#include <fstream>

#include <boost/any.hpp>

using namespace std;


//class ClassA {
//public:
//    int a;
//    string b;
//};
//
//void log_debug(boost::any obj) {
//   if (obj.type() == typeid(ClassA)) {
//      ClassA objA = boost::any_cast<ClassA>(obj);
//      cout << "found ClassA instance " << objA.a << ", " <<  objA.b << endl;
//   }
//
//}
//
//template<typename ObjectType>
//void template_method( const ObjectType& obj) {
//   cout << "-----------------------" << endl;
//   cout << "template_method, " << typeid(obj).name() << endl;
//
//   log_debug(obj);
//}

std::ofstream log_current_file;
uint32_t log_current_height;

int8_t log(uint32_t height, const string& content) {
//   if (height < log_current_height) {
//      // wtf, ignore
//      return 0;
//   }
//
//   string filepath = "/Volumes/RAMDisk/" + std::to_string(height) + ".txt";
//
//   // close log_current_file if height changed
//   if (height > log_current_height) {
//      if (log_current_file.is_open()) {
//         log_current_file.flush();
//         log_current_file.close();
//      }
//
//      // remove file if exist
//      std::remove(filepath.c_str());
//
//      // remember to update log_current_height
//      log_current_height = height;
//   }
//
//
//   if (!log_current_file.is_open()) {
//      // open new file
//      log_current_file.open(filepath, std::ios_base::app);
//   }
//
//   log_current_file << content << endl;

   return 0;
}

int main()
{
//   cout << "Hello, World!";

//   template_method("Hello, World!");
//   template_method(123);


//   ClassA objA;
//   objA.a = 123;
//   objA.b = "objA";
//
//   template_method(objA);


   log(0, "0_0");
   log(0, "0_1");
   log(0, "0_2");

   log(1, "1_0");
   log(1, "1_1");
   log(1, "1_2");

   log(2, "2_0");
   log(2, "2_1");
   log(2, "2_2");

   return 0;
}