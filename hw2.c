/* HW2 Template - Fall 2024 */

#include "hw2.h"
#include <stdio.h>
#include <string.h>

// Utility Bill Formula Coefficients
#define WATER_RATE 1.065
#define ELECTRICITY_RATE 0.0984
#define STEAM_RATE 0.06
#define TRASH_RATE 0.04

/* define calculate_bill here */

double calculate_bill(char *in_file, char *dorm_name) {
  FILE *file = fopen(in_file, "r");
  if (file == NULL) {
    return FILE_READ_ERR;
  }

  char dorm[50];
  int year;
  int month;
  int day;
  int residents;
  double water;
  int electricity;
  int steam;
  int trash;
  double total_bill = 0;
  int record_num = 0;

  //read in data from the input file
  int read_in = fscanf(file, "%d-%d-%d,\"%[^\"]\",%d\\%lf,%d,%d,%d", &year, &month, &day, dorm, &residents, &water, &electricity, &steam, &trash);
  //check for invalid date
  if ((month > 12) || (month < 1) || (day > 30) || (day < 1) || (year < 0)) {
    fclose(file);
    return BAD_DATE;
  }
  //check for the read data
  if (read_in != 9) {
    fclose(file);
    return BAD_RECORD;
  }

  while(read_in == 9) {
    if (strcmp(dorm, dorm_name) == 0) {
      double bill = WATER_RATE * water + ELECTRICITY_RATE * electricity + STEAM_RATE * steam + TRASH_RATE * trash;
      total_bill += bill;
      record_num++;
    }
  }

  if (ferror(file)) {
    fclose(file);
    return BAD_RECORD;
  }

  fclose(file);

  if (record_num == 0) {
    return NO_DATA_POINTS;
  }

  printf("Total bill for dorm %s: $%.2f\n", dorm_name, total_bill);
  return SUCCESS;
}

/* define generate_monthly_water_report here */

int generate_monthly_water_report(char *in_file, char *dorm_name, int year, char *out_file) {
  FILE *file = fopen(in_file, "r");
  if (!file) {
      return FILE_READ_ERR;
  }
  char dorm[50];
  int month;
  int day;
  int residents;
  int file_year;
  double water;
  int electricity;
  int steam;
  int trash;
  double monthly_usage[12] = {0};
  int valid_num = 0;

  int read_in = fscanf(file, "%d-%d-%d,\"%[^\"]\",%d\\%lf,%d,%d,%d", &file_year, &month, &day, dorm, &residents, &water, &electricity, &steam, &trash);
  if ((month > 12) || (month < 1) || (day > 30) || (day < 1) || (year < 0)) {
    fclose(file);
    return BAD_DATE;
  }
  if (read_in != 9) {
    fclose(file);
    return BAD_RECORD;
  }

  while(read_in == 9) {
    if (strcmp(dorm, dorm_name) == 0 && file_year == year) {
      monthly_usage[month - 1] += water;
      valid_num++;
    }
  }
  fclose(file);

  if (ferror(file)) {
    fclose(file);
    return BAD_RECORD;
  }
  if (valid_num == 0) {
    return NO_DATA_POINTS;
  }

  FILE *write_out = fopen(out_file, "w");
  if (!write_out) {
    return FILE_WRITE_ERR;
  }

  fprintf(write_out, "%s\n", dorm_name);
  for(int i = 0; i < 12; i++) {
    fprintf(write_out, "%02d: %.3f\n", i + 1, monthly_usage[i]);
  }
  fprintf(write_out, "%d\n", year);

  fclose(write_out);
  return SUCCESS;
}

/* define get_electricity_usage here */

int get_electricity_usage (char *in_file, char *dorm_name, int year) {
  FILE *file = fopen(in_file, "r");
  if (!file) {
    return FILE_READ_ERR;
  }
  char dorm[50];
  int month;
  int day;
  int residents;
  int file_year;
  float water;
  int electricity;
  int steam;
  int trash;
  double total_electricity = 0;
  int valid_num = 0;

  int read_in = fscanf(file, "%d-%d-%d,\"%[^\"]\",%d\\%f,%d,%d,%d", &file_year, &month, &day, dorm, &residents, &water, &electricity, &steam, &trash);
  if ((month > 12) || (month < 1) || (day > 30) || (day < 1) || (year < 0)) {
    fclose(file);
    return BAD_DATE;
  }
  if (read_in != 9) {
    fclose(file);
    return BAD_RECORD;
  }

  while (read_in == 9) {
    if (strcmp(dorm, dorm_name) == 0 && file_year == year) {
      total_electricity += electricity;
      valid_num++;
    }
  }

  if (ferror(file)) {
    fclose(file);
    return BAD_RECORD;
  }

  if (valid_num == 0) {
    return NO_DATA_POINTS;
  }

  printf("%.2f\n", total_electricity);
  return SUCCESS;
}

/* define get_average_resident_water_usage here */

double get_average_resident_water_usage (char *in_file, int month, int year) {
  FILE *file = fopen(in_file, "r");
  if (!file) {
    return FILE_READ_ERR;
  }
  char dorm[50];
  int residents;
  int file_year;
  int file_month;
  int day;
  double water;
  int electricity;
  int steam;
  int trash;
  double water_per_resident;
  double wr_total = 0;
  int valid_num = 0;

  int read_in = fscanf(file, "%d-%d-%d,\"%[^\"]\",%d\\%lf,%d,%d,%d", &file_year, &file_month, &day, dorm, &residents, &water, &electricity, &steam, &trash);
  if ((month > 12) || (month < 1) || (day > 30) || (day < 1) || (year < 0)) {
    fclose(file);
    return BAD_DATE;
  }
  if (read_in != 9) {
    fclose(file);
    return BAD_RECORD;
  }

  while(read_in == 9) {
    if ((file_year == year) && (file_month == month) && (residents > 0)) {
      water_per_resident = water / residents;
      wr_total += water_per_resident;
      valid_num++;
    } else if (residents == 0) {
      return NO_RESIDENTS;
    }
  }
  fclose(file);

  return wr_total / valid_num;

}
