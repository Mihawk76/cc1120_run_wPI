#
# Makefile:
#	wiringPi - Wiring Compatable library for the Raspberry Pi
#	https://projects.drogon.net/wiring-pi
#
#	Copyright (c) 2012-2013 Gordon Henderson
#################################################################################
# This file is part of wiringPi:
#	Wiring Compatable library for the Raspberry Pi
#
#    wiringPi is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################


#DEBUG	= -g -O0
DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lm -lcurl -lpthread -lrt
MYSQL_LIBS := $(shell mysql_config --libs --cflags)

###############################################################################

SRC	=	cc1120_run.c \
        equipment_alarm.c \
		kwh_params.c \
		crc16.c res_sensor.c paring.c

OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

#all:	$(BINS)

cc1120_run: cc1120_run.o equipment_alarm.o kwh_params.o crc16.o res_sensor.o paring.o
	@echo [link]
	@$(CC) -o $@ cc1120_run.o equipment_alarm.o kwh_params.o crc16.o res_sensor.o paring.o $(LDFLAGS) $(LDLIBS) $(MYSQL_LIBS)
	
.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@ $(MYSQL_LIBS)

clean:
	@echo "[Clean]"
	@rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

depend:
	makedepend -Y $(SRC)

# DO NOT DELETE
