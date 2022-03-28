import fetch from 'node-fetch'
import config from './config.js'
import { Client } from "discord.js";

const client = new Client()
client.login(config.discordToken).then(() => {
  console.log('[DISCORD] Bot logged as ' + client.user.username)
  setInterval(update, config.timeoutUpdate)
  update();
})


function update() {
  fetch(config.api_base + '/methods/getServer', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      api_token: config.api_token
    })
  })
    .then(res => {
      console.log('[FELIXBOT API] Info loaded')
      if(res.ok) {
        res.json()
          .then(({ data }) => {
            if(!data.server_online_new) return;
            if(!data.server_online_new.layout) return;
            let name = data.server_online_new.layout;
            name = name.replace("{online}", data.server_player_count);
            client.user.setPresence({
              status: 'online',
              activity: {
                name: name,
                type: "PLAYING",
              },
            })
          })
      }
    })
}
