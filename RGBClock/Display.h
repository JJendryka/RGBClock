struct Time {
  int seconds,
  int minutes,
  int hours
}

class Display {
public:
  Display()
  void setTime(Time time);
  void update()
private:
  Time time;
  RGBStrip strip;
}
