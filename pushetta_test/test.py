from pushetta import Pushetta
                  
API_KEY="21a6e54e956aa7526f638e6d0274c09307326434"
CHANNEL_NAME="Arduino_safety"
p=Pushetta(API_KEY)
p.pushMessage(CHANNEL_NAME, "Hallo Olaf!")