#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/init.h>

#define DEVICE_NAME     "lcd1602"
#define I2C_ADDR        0x27  // Địa chỉ I2C của PCF8574

static struct i2c_client *client;

static int lcd1602_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    printk("LCD 1602 driver probed\n");
    
    // Gửi dữ liệu "hello" đến màn hình LCD
    const char *hello = "hello";
    int ret;
    uint8_t data = 9;
    uint8_t buf[4] = {((data & 0xF0) | 0x04), ((data & 0xF0) | 0x00), ((data <<4) | 0x04), ((data <<4) | 0x00) };
	int i=0;
    for(i=0;i<4;i++){
  	ret = i2c_master_send(client, &buf[i], 1);
        if (ret < 0) {
            pr_err("Failed to send data to LCD: %d\n", ret);
            return ret;
        }

    }
    //ret = i2c_master_send(client, hello, strlen(hello));
   // if (ret < 0) {
    //    pr_err("Failed to send data to LCD: %d\n", ret);
    //    return ret;
    //}

    return 0;
}

static void lcd1602_remove(struct i2c_client *client) {
    pr_info("LCD 1602 driver removed\n");
    //return 0;
}

static const struct i2c_device_id lcd1602_id[] = {
    { "lcd1602", 0 },
    { }
};

MODULE_DEVICE_TABLE(i2c, lcd1602_id);

static struct i2c_driver lcd1602_driver = {
    .driver = {
        .name = DEVICE_NAME,
    },
    .probe = lcd1602_probe,
    .remove = lcd1602_remove,
    .id_table = lcd1602_id,
};

static int __init lcd1602_init(void) {
    int ret;

    // Đăng ký driver I2C
    ret = i2c_add_driver(&lcd1602_driver);
    if (ret != 0) {
        pr_err("Failed to register LCD 1602 driver: %d\n", ret);
        return ret;
    }

    pr_info("LCD 1602 driver registered\n");

    return 0;
}

static void __exit lcd1602_exit(void) {
    // Hủy đăng ký driver I2C
    i2c_del_driver(&lcd1602_driver);
    pr_info("LCD 1602 driver unregistered\n");
}

module_init(lcd1602_init);
module_exit(lcd1602_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Driver for controlling LCD 1602 via I2C");

