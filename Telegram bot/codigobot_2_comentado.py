#librerias necesarias para realizar el programa
import json
import logging
import sys
import time
#libreria para coger los datos csv almacenados en thingspeak
import thingspeak
#libreria para tratar con telegrams
import telepot
#para separar los datos de la fecha para poder darle la vuelta por que viene la fecha en formato americano
import re
#para poder abrir el enlace
import webbrowser
from urllib import request
from telepot.loop import MessageLoop
from telepot.namedtuple import InlineKeyboardMarkup, InlineKeyboardButton

#codigo de administrador para controlar el bot de telegram
TOKEN = '1717181750:AAGSxp6I0nnd5iuaaDu-GYxC8czugG-GBKk'

#función para crear los botones de opciones en el canal de telegram
def mensaje_chat(msg):
    #utilizo el comando glance de la libreria telepot para sacar los tres datos
    content_type, chat_type, chat_id = telepot.glance(msg)
    #para crear todos los botones que se mostraran en la pantalla de telegram
    keyboard = InlineKeyboardMarkup(inline_keyboard=[
                   [InlineKeyboardButton(text='Data visualization', url='https://thingspeak.com/channels/1326912')],
                   [InlineKeyboardButton(text='Soil moisture', callback_data='mois')],
               ])
    #despues de crear los botones publicas un mensaje en telegram(bot.sendmsg) y acto seguido manda keyboard para que marques una de las opciones
    bot.sendMessage(chat_id, 'Usa el menú para decidir lo que deseas mostrar', reply_markup=keyboard)



#La funcion coge de los datos almacenados en thingspeak el dato que pidas en telegram de acuerdo al boton pulsado
def datos_thingspeak(msg):
    #guardamos en esta variable hacia donde esta nuestro canal
    datos_thingspeak_brutos = thingspeak.Channel(1326912)
    #extraemos todos los datos del canal en formato csv
    ch = datos_thingspeak_brutos.get('{results}')
    #convertimos los datos de formato csv a formato json para poder acceder a todos los sub-datos almacenados(voltaje, potencia, etc..)
    data_dict = json.loads(ch)

    #separate the value that you need

    feeds = data_dict['feeds']

    #para separar y reconocer el boton que se apreto (query_data)
    
    query_id, from_id, query_data = telepot.glance(msg, flavor='callback_query')

    #-1 para que coja el ultimo dato disponible en thingspeak
    i = -1
    #coges el dato de dentro de json el que es sobre cuando se publico el ultimo dato
    date = feeds[i]['created_at']
    # para separar los datos sobre la fecha que se genero el dato de los de a que hora se genero el dato, obteniendo asi dos posiciones
    dateBien = re.split('T|Z', date)
    #para poder darle la vuelta al formato ingles de thingspeak separamos los datos segun los guiones generando asi un array de tres datos (año, mes y dia)
    dateBienDia = re.split('-', dateBien[0])
    #segun el boton apretado entra en una de las condiciones e imprime los datos de thingspeak
    if(query_data == 'mois'):
        bot.sendMessage(from_id, text='Reading from ' + dateBienDia[2] + '/' + dateBienDia[1] + '/' + dateBienDia[0] + ' at ' + dateBien[1] + '.\nSoil moisture = ' + str(round(float(feeds[i]['field1']), 2)) + ' x')
    elif(query_data == 'pact'):
        bot.sendMessage(from_id, text='Medicición hecha el ' + dateBienDia[2] + '/' + dateBienDia[1] + '/' + dateBienDia[0] + ' a las ' + dateBien[1] + '.\nP=' + str(round(float(feeds[i]['field2']), 2))+ ' (W)')
    elif(query_data == 'preact'):
        bot.sendMessage(from_id, text='Medicición hecha el ' + dateBienDia[2] + '/' + dateBienDia[1] + '/' + dateBienDia[0] + ' a las ' + dateBien[1] + '.\nQ=' + str(round(float(feeds[i]['field3']), 2))+ ' (VAr)')
    elif(query_data == 'fdp'):
        bot.sendMessage(from_id, text='Medicición hecha el ' + dateBienDia[2] + '/' + dateBienDia[1] + '/' + dateBienDia[0] + ' a las ' + dateBien[1] + '.\nFactor de Potencia = ' + str(round(float(feeds[i]['field4']), 2)))
    elif(query_data == 'consu'):
        bot.sendMessage(from_id, text='Medicición hecha el ' + dateBienDia[2] + '/' + dateBienDia[1] + '/' + dateBienDia[0] + ' a las ' + dateBien[1] + '.\nConsumo = ' + str(round(float(feeds[i]['field5']), 2))+ ' (Wh)')
    elif(query_data == 'calculo'):
        bot.sendMessage(from_id, text='Introduce el consumo')
        #bot.sendMessage(from_id, text='Medicición hecha el ' + dateBienDia[2] + '/' + dateBienDia[1] + '/' + dateBienDia[0] + ' a las ' + dateBien[1] + '.\nConsumo = ' + str(round(float(feeds[i]['field5']), 2))+ ' (Wh)')

#Inicializamos las funciones y ponemos a la escucha a la espera de llegada de actualizaciones
bot = telepot.Bot(TOKEN)
MessageLoop(bot, {'chat': mensaje_chat,
                  'callback_query': datos_thingspeak}).run_as_thread()
print('Waiting for keystrokes...')
while 1:
    time.sleep(10)