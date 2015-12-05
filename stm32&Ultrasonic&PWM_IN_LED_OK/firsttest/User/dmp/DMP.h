#include  "inv_mpu.h"
#include  "inv_mpu_dmp_motion_driver.h"
//#include  "common.h"

void dmp_init(void);
// static inline unsigned short inv_row_2_scale(const signed char *row);
// static inline unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
// static inline void run_self_test(void);	//wfh commited 15.10.23
static unsigned short inv_row_2_scale(const signed char *row);
static unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
static void run_self_test(void);
static void gyro_data_ready_cb(void);
