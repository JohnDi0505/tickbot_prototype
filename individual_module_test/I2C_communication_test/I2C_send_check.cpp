#include <iostream>

	struct data_frame_format
	{
		int		data01;
		float	data02;
		double	data03,data04;
		int		data05[6];
		char	data06[20];
		unsigned char check_code;
	};

#define SIZE	sizeof(data_frame_format)

	union data_field
	{
		data_frame_format	data_frame;
		unsigned char	transfer_char[SIZE];
	};


int main()
{
	data_field send_buffer;
	data_field receiver_buffer;

	send_buffer.data_frame.data01 = 100;
	send_buffer.data_frame.data02 = 3.14159f;
	send_buffer.data_frame.data03 = 1.23456789;
	send_buffer.data_frame.data04 = 9.87654321;
	int	i;
	for (i = 0; i < 6; i++) { send_buffer.data_frame.data05[i] = i + 1; }
	for (i = 0; i < 19; i++) { send_buffer.data_frame.data06[i] = i + 'a'; } send_buffer.data_frame.data06[i] = '\0';

	unsigned char	n=0;
	for (i = 0; i < SIZE - 1; i++) { n += send_buffer.transfer_char[i]; }
	send_buffer.data_frame.check_code = -n;	//Set check code byte


	for (i = 0; i < SIZE; i++) { receiver_buffer.transfer_char[i] = send_buffer.transfer_char[i]; }	//Transfer data...


	unsigned char	m = 0;
	for (i = 0; i < SIZE; i++) { m += receiver_buffer.transfer_char[i]; }	//Checking
	if (m == 0) std::cout << "Transmission right" << std::endl;
	else std::cout << "transmission error" << std::endl;
	std::cout << std::endl;

	std::cout << receiver_buffer.data_frame.data01 << std::endl;
	std::cout << receiver_buffer.data_frame.data02 << std::endl;
	std::cout << receiver_buffer.data_frame.data03 << std::endl;
	std::cout << receiver_buffer.data_frame.data04 << std::endl;
	for (i = 0; i < 6; i++) { std::cout << receiver_buffer.data_frame.data05[i]<<" "; }
	std::cout << std::endl;
	std::cout << receiver_buffer.data_frame.data06 <<std::endl;
	std::cout << (int)m << std::endl;	//Display check results
}
