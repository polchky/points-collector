#ifndef Volley_h
#define Volley_h


class Volley {
  public:
    Volley(uint8_t score_0, uint8_t score_1, uint8_t score_2);
    void setScore(uint8_t index, uint8_t score);
    void setScores(uint8_t score_0, uint8_t score_1, uint8_t score_2);
    void decreaseScore(uint8_t index);
    uint8_t getScore(uint8_t index);
    void setTimestamp(unsigned long offset);
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

void Volley::decreaseScore(uint8_t index)
{
  if(_scores[index] > 6){
    _scores[index]--;
  }else{
    _scores[index] = 0;
  }
}

uint8_t Volley::getScore(uint8_t index)
{
  return _scores[index];
}

void Volley::setTimestamp(unsigned long offset)
{
  _timestamp = (millis() + offset) / 1000;
}

unsigned long Volley::getTimestamp()
{
  return _timestamp;
}

#endif
