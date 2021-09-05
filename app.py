from flask import Flask, render_template
import re
import csv
import os, sys
import serial
import time
from datetime import date

app = Flask(__name__)

@app.route('/')
def index():

    # Get current date
    today = date.today()
    # mm/dd/y
    d = today.strftime("%m/%d/%y")
    print("Date =", d)

    sensorData = []
    moistureValues = []

    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout = 10)

    # Store data of each sensor read from serial port
    for i in range(0,6):
        sensorData.append(ser.readline().decode())

        if len(sensorData) == 0:
            print("Time out. Exit \n")
            sys.exit()

    print(sensorData) 
    
    # Extract only the moisture values from incoming data
    extractMoisture = r':\s[0-9]+'
    regex1 = re.compile(extractMoisture)
   
    for i, sensor in enumerate(sensorData):
        moisture = regex1.findall(sensorData[i])
        print(moisture)
        moistureString = str(moisture[0])
        moistureInt = int(moistureString[2:])

        moistureValues.append(moistureInt)
   
    print(moistureValues)


    # Store sensor data in csv file 
    file_path = 'fieldData.csv'

    rows = []
    currentRow = []


    # fieldsExist = True
    # # Reading first line of csv file
    # with open(file_path, 'r') as csvfile:
    #     csvreader = csv.reader(csvfile)
    #     for line in csvreader:
    #         print(line)
    #         if line == None:
    #             fieldsExist == False
    #             break



    # Write to csv file
    with open(file_path, 'a') as csvfile:
        csvwriter = csv.writer(csvfile)
       
        # if fieldsExist == False:
        #     # Create fields: Date, Sensor1, Sensor2,...
        #     fields = ['Date']
        #     for sensor in range(0, len(moistureValues)):
        #         fields.append('Sensor' + str(sensor + 1))
           
        #     # Writing the fields
        #     csvwriter.writerow(fields)
       

        # Create the new row: Date, Value1, Value2,...
        currentRow.append(d)
        for sensorValue in moistureValues:
            currentRow.append(sensorValue)

        print(currentRow)
        # Append the current data to csv file
        csvwriter.writerow(currentRow)
   

    # Read all rows from csv file, store into an array of rows
    with open(file_path, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            for i,elm in enumerate(row):
                if i != 0:
                    row[i] = int(row[i])
            rows.append(row)
        print(rows)
   


  # Variables to send to javascript code:
  # currentRow stores incoming data, rows stores all previous data
  # Use them for Google Charts

    return render_template("index.html", rows = rows, currentRow = currentRow )

