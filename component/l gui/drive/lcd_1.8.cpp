/**
  ******************************************************************************
  * @file    lcd_1.8.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
/*******************************
1.8��Һ����Ļ��������
����IC:ST7735S
�ӿڣ�SPI��MODE2��3��
�ֱ��ʣ�128*160
ɫ�ʣ�256K
*******************************/
/******************************
    GND ->GND
    VCC -> 3.3V
    SCL -> PA5
    SDA -> PA7
    RES -> PB3
    DC  -> PB4
    CS  -> PB5
    BL  -> PB6
//����������eBox
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB3 (RST)
//              DC    ��PB4 (RS)
//              CS    ��PB5
//              BL    ��PB6

*******************************/
#include "lcd_1.8.h"

//Lcd lcd(&PB13,&PB15,&PA13,&PA15,&PA14,&PA12);

void Lcd::begin()
{

    width = X_MAX_PIXEL;
    height = Y_MAX_PIXEL;
    cs->mode(OUTPUT_PP);
    led->mode(OUTPUT_PP);
    rs->mode(OUTPUT_PP);
    rst->mode(OUTPUT_PP);
    led->set();

    if(initialized == 0)
    {
        config.mode = Spi::MODE2;
        config.bit_order = Spi::MSB;
        config.dev_num = cs->id;
        config.prescaler = Spi::DIV2;
    }
    spi->begin(&config);

    init();
}
void Lcd::soft_reset()
{
    write_index(0x01);//soft reset
}

void Lcd::on()
{
    write_index(0x29);//Display on
}
void Lcd::off()
{
    write_index(0x28);//Display off

}
//void Lcd::column_order(uint8_t order)
//{
//    write_index(0x36); //MX, MY, RGB mode
//    if(order == 1)
//    {
//        MADCTL |= 1 << 6;
//    }
//    else
//        MADCTL &= ~(1 << 6);
//    write_data_8bit(MADCTL);

//}
//void Lcd::row_order(uint8_t order)
//{
//    write_index(0x36); //MX, MY, RGB mode
//    if(order == 1)
//    {
//        MADCTL |= 1 << 7;
//    }
//    else
//        MADCTL &= ~(1 << 7);
//    write_data_8bit(MADCTL);

//}

/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd::clear(uint16_t Color)
{
    unsigned int i;
    set_region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
    write_index(0x2C);
    for(i = 0; i < X_MAX_PIXEL * Y_MAX_PIXEL; i++)
    {
        write_data_16bit(Color);
    }
}
/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd::set_xy(int16_t x, int16_t y)
{
    set_region(x, y, x, y);
}



void Lcd::draw_pixel(int16_t x, int16_t y, E_COLOR color)
{
    if(x < 0 || x > width) return;
    if(y < 0 || y > height) return;
    set_region(x, y, x + 1, y + 1);
    write_data_16bit(color);
}


/*************************************************
��������draw_h_line
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Lcd::draw_h_line(int16_t x0, int16_t y0, int16_t x1, E_COLOR color)
{
    if(x0 < 0 || x0 > width) return;
    if(y0 < 0 || y0 > height) return;

    set_region(x0, y0,x0+ x1, y0);
    for (x0=0 ; x0 <= x1; x0++)
    {
        write_data_16bit(color);
    }
}

/*************************************************
��������draw_v_line
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Lcd::draw_v_line(int16_t x0, int16_t y0,  int16_t y1, E_COLOR color)
{

    if(x0 < 0 || x0 > width) return;
    if(y0 < 0 || y0 > height) return;

    set_region(x0, y0, x0, y0+y1);
    for (y0=0; y0 <= y1; y0++)
    {
        write_data_16bit(color);
    }
}

void Lcd::fill_rect(int16_t x0, int16_t y0,  int16_t x1, int16_t y1, E_COLOR color)
{
    uint16_t i = 0;
    uint8_t dx, dy;
    dx = x1  + 1;
    dy = y1  + 1;

    set_region(x0, y0, x0+x1, y0+y1);
    for (; i <= dx * dy; i++)
    {
        write_data_16bit(color);
    }
}
void Lcd::fill_rect(int16_t x0, int16_t y0,  int16_t x1, int16_t y1, uint16_t *bitmap)
{
    uint16_t i = 0;
    uint8_t dx, dy;
    dx = x1  + 1;
    dy = y1  + 1;

    set_region(x0, y0, x0+x1, y0+y1);
    for (; i <= dx * dy; i++)
    {
        write_data_16bit(bitmap[i]);
    }

}
void Lcd::fill_screen(E_COLOR color)
{
    unsigned int i;
    set_region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
    write_index(0x2C);
    for(i = 0; i < X_MAX_PIXEL * Y_MAX_PIXEL; i++)
    {
        write_data_16bit(color);
    }
}

//���ߺ�����ʹ��Bresenham �����㷨
void Lcd::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, E_COLOR color)
{
    int dx,             // difference in x's
        dy,             // difference in y's
        dx2,            // dx,dy * 2
        dy2,
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error,          // the discriminant i.e. error i.e. decision variable
        index;          // used for looping


    set_xy(x0, y0);
    dx = x1 - x0; //����x����
    dy = y1 - y0; //����y����

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx    = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy    = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy)//x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
    {
        //���ߵĵ�������x���룬��x���������
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) //Ҫ���ĵ������ᳬ��x����
        {
            //����
            draw_pixel(x0, y0, color);

            // test if error has overflowed
            if (error >= 0) //�Ƿ���Ҫ����y����ֵ
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; //����y����ֵ
            } // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; //x����ֵÿ�λ���󶼵���1
        } // end for
    } // end if |slope| <= 1
    else//y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
    {
        //��y��Ϊ��������
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++)
        {
            // set the pixel
            draw_pixel(x0, y0, color);

            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;

                // move to next line
                x0 += x_inc;
            } // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
        } // end for
    } // end else |slope| > 1
}
void Lcd::draw_circle(int16_t x, int16_t y, int16_t r, E_COLOR color)
{
    unsigned short  a, b;
    int c;
    a = 0;
    b = r;
    c = 3 - 2 * r;
    while (a < b)
    {
        draw_pixel(x + a, y + b, color); //        7
        draw_pixel(x - a, y + b, color); //        6
        draw_pixel(x + a, y - b, color); //        2
        draw_pixel(x - a, y - b, color); //        3
        draw_pixel(x + b, y + a, color); //        8
        draw_pixel(x - b, y + a, color); //        5
        draw_pixel(x + b, y - a, color); //        1
        draw_pixel(x - b, y - a, color); //        4

        if(c < 0) c = c + 4 * a + 6;
        else
        {
            c = c + 4 * (a - b) + 10;
            b -= 1;
        }
        a += 1;
    }
    if (a == b)
    {
        draw_pixel(x + a, y + b, color);
        draw_pixel(x + a, y + b, color);
        draw_pixel(x + a, y - b, color);
        draw_pixel(x - a, y - b, color);
        draw_pixel(x + b, y + a, color);
        draw_pixel(x - b, y + a, color);
        draw_pixel(x + b, y - a, color);
        draw_pixel(x - b, y - a, color);
    }

}



//��Һ����дһ��8λָ��
void Lcd::write_index(uint8_t Index)
{
    //SPI д����ʱ��ʼ
    spi->take(&config);
    cs->reset();
    rs->reset();
    spi->write(Index);
    cs->set();
    spi->release();
}
//��Һ����дһ��8λ����
void Lcd::write_data_8bit(uint8_t Data)
{
    spi->take(&config);
    cs->reset();
    rs->set();
    spi->write(Data);
    cs->set();
    spi->release();
}
//��Һ����дһ��16λ����
void Lcd::write_data_16bit(uint16_t Data)
{
    spi->take(&config);
    cs->reset();
    rs->set();
    spi->write(Data >> 8); 	//д���8λ����
    spi->write(Data); 			//д���8λ����
    cs->set();
    spi->release();
}

void Lcd::write_reg(uint8_t Index, uint8_t Data)
{
    write_index(Index);
    write_data_8bit(Data);
}
/*************************************************
��������set_region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd::set_region(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end)
{
    write_index(0x2a);
    write_data_8bit(0x00);
    write_data_8bit(x_start + 2);
    write_data_8bit(0x00);
    write_data_8bit(x_end + 2);

    write_index(0x2b);
    write_data_8bit(0x00);
    write_data_8bit(y_start + 1);
    write_data_8bit(0x00);
    write_data_8bit(y_end + 1);

    write_index(0x2c);

}
void Lcd::reset(void)
{
    rst->reset();
    delay_ms(1000);
    rst->set();
    delay_ms(50);
}
//Lcd Init For 1.44Inch Lcd Panel with ST7735R.
void Lcd::init(void)
{
    reset(); //Reset before Lcd Init.

    //Lcd Init For 1.44Inch Lcd Panel with ST7735R.
    write_index(0x11);//Sleep exit
    delay_ms (120);

    //ST7735R Frame Rate
    write_index(0xB1);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);

    write_index(0xB2);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);

    write_index(0xB3);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);

    write_index(0xB4); //Column inversion
    write_data_8bit(0x07);

    //ST7735R Power Sequence
    write_index(0xC0);
    write_data_8bit(0xA2);
    write_data_8bit(0x02);
    write_data_8bit(0x84);
    write_index(0xC1);
    write_data_8bit(0xC5);

    write_index(0xC2);
    write_data_8bit(0x0A);
    write_data_8bit(0x00);

    write_index(0xC3);
    write_data_8bit(0x8A);
    write_data_8bit(0x2A);
    write_index(0xC4);
    write_data_8bit(0x8A);
    write_data_8bit(0xEE);

    write_index(0xC5); //VCOM
    write_data_8bit(0x0E);

//    MADCTL = 0XC0;
    write_index(0x36); //MX, MY, RGB mode
    write_data_8bit(0XC0);

    //ST7735R Gamma Sequence
    write_index(0xe0);
    write_data_8bit(0x0f);
    write_data_8bit(0x1a);
    write_data_8bit(0x0f);
    write_data_8bit(0x18);
    write_data_8bit(0x2f);
    write_data_8bit(0x28);
    write_data_8bit(0x20);
    write_data_8bit(0x22);
    write_data_8bit(0x1f);
    write_data_8bit(0x1b);
    write_data_8bit(0x23);
    write_data_8bit(0x37);
    write_data_8bit(0x00);
    write_data_8bit(0x07);
    write_data_8bit(0x02);
    write_data_8bit(0x10);

    write_index(0xe1);
    write_data_8bit(0x0f);
    write_data_8bit(0x1b);
    write_data_8bit(0x0f);
    write_data_8bit(0x17);
    write_data_8bit(0x33);
    write_data_8bit(0x2c);
    write_data_8bit(0x29);
    write_data_8bit(0x2e);
    write_data_8bit(0x30);
    write_data_8bit(0x30);
    write_data_8bit(0x39);
    write_data_8bit(0x3f);
    write_data_8bit(0x00);
    write_data_8bit(0x07);
    write_data_8bit(0x03);
    write_data_8bit(0x10);

    write_index(0x2a);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x7f);

    write_index(0x2b);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x9f);

    write_index(0xF0); //Enable test command
    write_data_8bit(0x01);
    write_index(0xF6); //Disable ram power save mode
    write_data_8bit(0x00);

    write_index(0x3A); //65k mode
    write_data_8bit(0x05);

    ///////////////////////////
    write_index(0x0c);
    write_data_8bit(0x00);
    write_data_8bit(0x05);
    /////////////////////////////////
    write_index(0x29);//Display on
}
