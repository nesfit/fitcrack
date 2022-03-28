#ifndef TRICKLE_HPP
#define TRICKLE_HPP

class Trickle {
  std::string trickle_xml_;

public:
  Trickle(){};

  void addElement(std::string tag, const std::string value) {
    trickle_xml_ += "<" + tag + ">" + value + "</" + tag + ">" + "\n";
  }

  void addElement(std::string tag, const int64_t value) {
    trickle_xml_ +=
        "<" + tag + ">" + std::to_string(value) + "</" + tag + ">" + "\n";
  }

  const std::string &getXml() { return trickle_xml_; }
};

#endif // TRICKLE_HPP