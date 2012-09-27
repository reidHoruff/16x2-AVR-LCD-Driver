#include <avr/io.h>
#include <util/delay.h>

#define REG_SELECT_DDR DDRB
#define REG_SELECT_PORT PORTB
#define REG_SELECT_PIN 0

#define READ_WRITE_DDR DDRB
#define READ_WRITE_PORT PORTB
#define READ_WRITE_PIN 1

#define ENABLE_LINE_DDR DDRB
#define ENABLE_LINE_PORT PORTB
#define ENABLE_LINE_PIN 2

#define COM_DDR DDRD
#define COM_PORT PORTD

#define DATA 1
#define COMMAND 0

#define READ 1
#define WRITE 0

#define clear_lcd() write_cmd(0x01)
#define turn_on_lcd() write_cmd(0x0C)
#define power_sold_cursor() write_cmd(0x0E)
#define power_blink_cursor() write_cmd(0x0F)

#define set_cursor(x) write_cmd(line_cmd[x-1])

uint8_t line_cmd[] = {0x80, 0xC0, 0x94, 0xD4};

void write_dat(char);
void write_cmd(char);

void init_lcd()
{
	_delay_ms(50);
	/*wait for LCD to boot*/

	REG_SELECT_DDR |= _BV(REG_SELECT_PIN);
	READ_WRITE_DDR |= _BV(READ_WRITE_PIN);
	ENABLE_LINE_DDR |= _BV(ENABLE_LINE_PIN);
	COM_DDR = 0xFF;

	COM_PORT = 0x30;
	REG_SELECT_PORT &= ~(1<<REG_SELECT_PIN);

	_delay_ms(5);

	COM_PORT = 0x30;
	REG_SELECT_PORT &= ~(1<<REG_SELECT_PIN);

	_delay_ms(1);

	COM_PORT = 0x30;
	REG_SELECT_PORT &= ~(1<<REG_SELECT_PIN);

	_delay_ms(1);

	write_cmd(0x38);
	write_cmd(0x08);
	write_cmd(0x01);
	write_cmd(0x06);
	write_cmd(0x0C);
}

void write_dat(char c)
{
	ENABLE_LINE_PORT &= ~(1<<ENABLE_LINE_PIN);
	REG_SELECT_PORT |= 1<<REG_SELECT_PIN;
	COM_PORT = c;
	ENABLE_LINE_PORT |= 1<<ENABLE_LINE_PIN;
	_delay_us(500);
	ENABLE_LINE_PORT &= ~(1<<ENABLE_LINE_PIN);
	_delay_us(200);
}

void write_cmd(char c)
{
	ENABLE_LINE_PORT &= ~(1<<ENABLE_LINE_PIN);
	REG_SELECT_PORT &= ~(1<<REG_SELECT_PIN);
	COM_PORT = c;
	ENABLE_LINE_PORT |= 1<<ENABLE_LINE_PIN;
	_delay_us(500);
	ENABLE_LINE_PORT &= ~(1<<ENABLE_LINE_PIN);
	_delay_ms(5);
}

void print(char *c, uint8_t p)
{
	set_cursor(p);

	for(uint8_t x = 0; x < strlen(c); x++){
		write_dat(c[x]);
	}
}

/* example
int main()
{
	init_lcd();
	print("HELLO CATROID!",1);
	print("2011 GSOC",2);
	return 0;
}
*/
