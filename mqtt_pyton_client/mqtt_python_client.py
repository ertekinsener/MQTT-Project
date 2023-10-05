import paho.mqtt.client as mqtt

broker_address = "localhost"
port = 1883

client = mqtt.Client()


client.connect(broker_address, port)


topics = ["/system_state", "/led_state_1", "/led_state_2", "/button_press_count"]


def on_message(client, userdata, message):
    print(f"Received message on topic {message.topic}: {message.payload.decode()}")


client.on_message = on_message


for topic in topics:
    client.subscribe(topic)


client.loop_forever()
