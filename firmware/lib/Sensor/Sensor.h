#include <bsec.h>

const uint8_t iaqConfig[] = {
  #include "config/generic_33v_3s_4d/bsec_iaq.txt"
};

extern bsec_virtual_sensor_t sensorFields[];

struct Sensor {
  Bsec* sensor;

  Sensor();
  ~Sensor();

  bool init();
  bool check();
  bool read();
};
