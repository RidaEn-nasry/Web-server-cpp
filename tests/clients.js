

// creating a client 
const http = require('http');


// creatign a clinet 
const client = http.createClient(8080, 'localhost:8080');

// creating a request
const request = client.request('GET', '/cgi-bin/generate-image.js', {
    'Host': 'localhost:8080'
});

let headers = '';
let body = '';


// sending the request
request.end();


