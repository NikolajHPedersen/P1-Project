add_library(file_reader.c file_reader)
add_library(schedule_lib.c schedule_lib file_reader)
add_library(appointment_system.c appointment_system file_reader)


add_executable(main.c main appointment_system file_reader schedule_lib)
