// take the argument 

const args = process.argv;
const text = args[2];

const API_KEY = "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"
const axios = require("axios");

axios({
    method: 'post',
    url: 'https://api.openai.com/v1/images/generations',

    headers: {
        Authorization: `Bearer ${API_KEY}`,
        'Content-Type': 'application/json'
    },
    data: {
        "prompt": text,
        "n": 1,
        "size": "1024x1024",
    }
}).then((response) => {
    const url = response['data'];
    console.log(url.data[0].url);
}).catch((error) => {
    url = "https://i.kym-cdn.com/entries/icons/original/000/021/464/14608107_1180665285312703_1558693314_n.jpg";
    console.log(url);
});