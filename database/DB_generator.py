import datetime
import sqlite3

import paho.mqtt.client as mqtt

# https://inloop.github.io/sqlite-viewer/

MQTT_HOST = 'test.mosquitto.org'
MQTT_PORT = 1883
MQTT_CLIENT_ID = 'Python MQTT client'
TOPIC = 'g2/#'

DATABASE_FILE = 'mqtt_entries.db'







def on_connect(mqtt_client, user_data, flags, conn_result):
    mqtt_client.subscribe(TOPIC)


def on_message(mqtt_client, user_data, message):
    if message.topic == 'g2/temp':
        payload = message.payload.decode('utf-8')
        db_conn = user_data['db_conn']
        temp = 'INSERT INTO Temperature (topic, payload, created_at) VALUES (?, ?, ?)'
        cursor = db_conn.cursor()
        cursor.execute(temp, (message.topic, payload, datetime.datetime.now().strftime("%m/%d/%Y, %H:%M:%S")))
        db_conn.commit()
        cursor.close()
    if message.topic == 'g2/light':
        payload = message.payload.decode('utf-8')
        db_conn = user_data['db_conn']
        light = 'INSERT INTO Light (topic, payload, created_at) VALUES (?, ?, ?)'
        cursor = db_conn.cursor()
        cursor.execute(light, (message.topic, payload, datetime.datetime.now().strftime("%m/%d/%Y, %H:%M:%S")))
        db_conn.commit()
        cursor.close()
    if message.topic == 'g2/humid':
        payload = message.payload.decode('utf-8')
        db_conn = user_data['db_conn']
        humid = 'INSERT INTO Humidity (topic, payload, created_at) VALUES (?, ?, ?)'
        cursor = db_conn.cursor()
        cursor.execute(humid, (message.topic, payload, datetime.datetime.now().strftime("%m/%d/%Y, %H:%M:%S")))
        db_conn.commit()
        cursor.close()
    if message.topic == 'g2/gps':
        payload = message.payload.decode('utf-8')
        db_conn = user_data['db_conn']
        gps = 'INSERT INTO GPS (topic, payload, created_at) VALUES (?, ?, ?)'
        cursor = db_conn.cursor()
        cursor.execute(gps, (message.topic, payload, datetime.datetime.now().strftime("%m/%d/%Y, %H:%M:%S")))
        db_conn.commit()
        cursor.close()


def main():
    db_conn = sqlite3.connect(DATABASE_FILE, detect_types=sqlite3.PARSE_DECLTYPES |
                                                          sqlite3.PARSE_COLNAMES)
    temp = """
    CREATE TABLE IF NOT EXISTS Temperature (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        topic TEXT NOT NULL,
        payload TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )
    """
    light = """
        CREATE TABLE IF NOT EXISTS Light (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            topic TEXT NOT NULL,
            payload TEXT NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
        """
    humid = """
            CREATE TABLE IF NOT EXISTS Humidity (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                topic TEXT NOT NULL,
                payload TEXT NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
            """

    gps = """
            CREATE TABLE IF NOT EXISTS GPS (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                topic TEXT NOT NULL,
                payload TEXT NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
            """
    cursor = db_conn.cursor()
    cursor.execute(temp)
    cursor.execute(light)
    cursor.execute(humid)
    cursor.execute(gps)
    cursor.close()

    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    mqtt_client.user_data_set({'db_conn': db_conn})

    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_HOST, MQTT_PORT)
    mqtt_client.loop_forever()


main()
