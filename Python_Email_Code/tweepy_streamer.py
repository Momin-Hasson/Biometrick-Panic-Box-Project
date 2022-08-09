# Biometric Panic Box Twitter API Tweet Tracking and Email notification
# Author: Group 19 CS 362 
# Date: 4/10/2021
# Tutorials referenced: https://www.youtube.com/watch?v=wlnx-7cm4Gg
# Tweepy Documentation: https://docs.tweepy.org/en/latest/
# 
# Description: StreamListener listens to active twitter feed for tweets with the "#biometricpanicbox" hashtag, upon receiving the JSON
#              data from the tweet, we parse only the tweet content from the data and then we email the contents to a predetermined list of 
#              emails which can be found in the msg['To'] list

from tweepy.streaming import StreamListener
from tweepy import OAuthHandler
from tweepy import Stream
import json
import twitter_credentials
import smtplib
from email.message import EmailMessage

class StdOutListener(StreamListener):
    def on_data(self, data):
        try:   
            json_load = json.loads(data)                # Load the JSON data file from twitter
            tweet = json_load['text']                   # Parse the tweet out from the file
            
            # start email process
            SenderEmail = 'insert email here' 
            SenderPassword = 'insert password here'
            msg = EmailMessage()
            msg['Subject'] = 'Biometric Panic Box'
            msg['From'] = SenderEmail
            msg['To'] = 'gagegoodwin123@gmail.com'
            msg.set_content(tweet)                      # set the content of the message with the scraped tweet

            with smtplib.SMTP('smtp.office365.com', 587) as smtp: 
                smtp.ehlo()
                smtp.starttls()
                smtp.ehlo()
                smtp.login(SenderEmail, SenderPassword)
                smtp.send_message(msg)                 
            return True

        except BaseException as e:
            print("Error on_data %s" % str(e))          
        return True

    def on_error(self, status):
        print(status)

if __name__ == "__main__":
    listener = StdOutListener()
    auth = OAuthHandler(twitter_credentials.CONSUMER_KEY, twitter_credentials.CONSUMER_SECRET)
    auth.set_access_token(twitter_credentials.ACCESS_TOKEN, twitter_credentials.ACCESS_TOKEN_SECRET)

    stream = Stream(auth, listener)

    stream.filter(track=['0BWtARjU'])         # Only look for tweets with this string