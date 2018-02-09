#ifndef Volley_h
#define Volley_h


class Volley {
  public:
    Volley(uint8_t score_0, uint8_t score_1, uint8_t score_2);
    void setScore(uint8_t index, uint8_t score);
    void setScores(uint8_t score_0, uint8_t score_1, uint8_t score_2);
    void setTimestamp();
    unsigned long getTimestamp();
    
  private:
    unsigned long _timestamp;
    uint8_t _scores[3];
};

Volley::Volley(uint8_t score_0 = 10, uint8_t score_1 = 10, uint8_t score_2 = 10)
{
  _scores[0] = score_0;
  _scores[1] = score_1;
  _scores[2] = score_2;
  _timestamp = 0;
}

void Volley::setScore(uint8_t index, uint8_t score)
{
  _scores[index] = score;
}

void Volley::setScores(uint8_t score_0, uint8_t score_1, uint8_t score_2)
{
  _scores[0] = score_0;
  _scores[1] = score_1;
  _scores[2] = score_2;
}

void Volley::setTimestamp()
{
  _timestamp = millis() / 1000;
}

unsigned long Volley::getTimestamp()
{
  return _timestamp;
}

#endif
