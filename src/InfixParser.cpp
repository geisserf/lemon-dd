#include "InfixParser.h"
#include "catamorph/Factories.h"
#include <string>
#include <iostream>


std::string InfixParser::getNextToken() {
  if (this->currentIndex >= this->expression.size()) {
    return "";
  }

  //Add at least 1 elment to vector
  if (this->expression[this->currentIndex] == '+') {
    this->currentIndex++;
    return "+";
  } else if (this->expression[this->currentIndex] == '-') {
    this->currentIndex++;
    return "-";
  } else if (this->expression[this->currentIndex] == '*') {
    this->currentIndex++;
    return "*";
  } else if (this->expression[this->currentIndex] == '/') {
    this->currentIndex++;
    return "/";
  } else if (this->expression[this->currentIndex] == '(') {
    this->currentIndex++;
    return "(";
  } else if (this->expression[this->currentIndex] == ')') {
    this->currentIndex++;
    return ")";
  } else if (this->expression[this->currentIndex] == '[') {
    this->currentIndex++;
    return "[";
  } else if (this->expression[this->currentIndex] == ']') {
    this->currentIndex++;
    return "]";
  } else if (this->expression[this->currentIndex] == '=' &&
      this->expression[this->currentIndex+1] == '=') {
    this->currentIndex++;
    this->currentIndex++;
    return "==";
  }
  std::vector<char> v;

  for (; this->currentIndex < this->expression.size(); ++this->currentIndex) {
    if (this->expression[this->currentIndex] == '+') {
      break;
    } else if (this->expression[this->currentIndex] == '-') {
      break;
    } else if (this->expression[this->currentIndex] == '*') {
      break;
    } else if (this->expression[this->currentIndex] == '/') {
      break;
    } else if (this->expression[this->currentIndex] == '(') {
      break;
    } else if (this->expression[this->currentIndex] == ')') {
      break;
    } else if (this->expression[this->currentIndex] == '[') {
      break;
    } else if (this->expression[this->currentIndex] == ']') {
      break;
    } else if (this->expression[this->currentIndex] == '=') {
      //this->currentIndex++;//always ==
      break;
    }else if(this->expression[this->currentIndex] == '\0'){
      break;
    }else{
      v.push_back(this->expression[this->currentIndex]);
    }
  }

  return std::string(v.begin(), v.end());
}

Expression InfixParser::parse(std::string expression) {
  this->expression = expression;
  this->currentIndex = 0;

  std::string token;
  while ((token = this->getNextToken()) != "") {
    std::cout << token << std::endl;
  }
  return Factories::cst(0);
}