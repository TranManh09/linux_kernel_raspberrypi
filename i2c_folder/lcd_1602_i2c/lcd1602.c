#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/acpi.h> // Thêm thư viện này

#define I2C_BUS_AVAILABLE 1
#define DEVICE_NAME "lcd1602"
#define I2C_ADDR 0x27 // Địa chỉ I2C của PCF8574

static struct i2c_client *client;
static struct i2c_adapter *etx_i2c_adapter = NULL;    // I2C Adapter Structure
static struct i2c_client *etx_i2c_client_oled = NULL; // I2C Cient Structure (In our case it is OLED)

static int lcd1602_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    pr_info("LCD 1602 driver probed\n");

    // Gửi dữ liệu "hello" đến màn hình LCD
    int ret;
    uint8_t data = 'H';
    uint8_t buf[4] = {((data & 0xF0) | 0x04), ((data & 0xF0) | 0x00), ((data << 4) | 0x04), ((data << 4) | 0x00)};
    int i = 0;
    char temp = 0x04;
    char *a = &temp;
    for (i = 0; i < 4; i++)
    {
        ret = i2c_master_send(client, &buf[i], sizeof(buf[i]));
        if (ret < 0)
        {
            pr_err("Failed to send data to LCD: %d\n", ret);
            return ret;
        }
        else
        {
            pr_info("transmitted success\n");
        }
    }
    // ret = i2c_master_send(client, hello, strlen(hello));

    return 0;
}

static void lcd1602_remove(struct i2c_client *client)
{
    pr_info("LCD 1602 driver removed\n");
    // return 0;
}

static const struct i2c_device_id lcd1602_id[] = {
    { DEVICE_NAME, 0},
    {}};

MODULE_DEVICE_TABLE(i2c, lcd1602_id);

static struct i2c_driver lcd1602_driver = {
    .driver = {
        .name = DEVICE_NAME,
        .owner = THIS_MODULE
    },
    .probe = lcd1602_probe,
    .remove = lcd1602_remove,
    .id_table = lcd1602_id,
};

static struct i2c_board_info oled_i2c_board_info = {
    I2C_BOARD_INFO(DEVICE_NAME, I2C_ADDR)};

static int __init lcd1602_init(void)
{
    int ret = -1;
    etx_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (etx_i2c_adapter != NULL)
    {
        // Sử dụng macro I2C_BOARD_INFO để khởi tạo thông tin thiết bị
       // struct i2c_board_info board_info;
       // memset(&board_info, 0, sizeof(board_info));
       // strlcpy(board_info.type, DEVICE_NAME, I2C_NAME_SIZE);
       // board_info.addr = I2C_ADDR;

        etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);

        if (etx_i2c_client_oled != NULL)
        {
            i2c_add_driver(&lcd1602_driver);
            ret = 0;
        }

        i2c_put_adapter(etx_i2c_adapter);
    }

    pr_info("Driver Added!!!\n");
    return ret;
}

static void __exit lcd1602_exit(void)
{
    // Hủy đăng ký driver I2C
    i2c_del_driver(&lcd1602_driver);
    pr_info("LCD 1602 driver unregistered\n");
}

module_init(lcd1602_init);
module_exit(lcd1602_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Driver for controlling LCD 1602 via I2C");
