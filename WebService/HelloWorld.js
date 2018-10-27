var http = require('http');
var os   = require('os');
var url  = require('url');
var fs   = require('fs');

var context = {
    address     : null,
    listenPort  : 80,

    printDebug  : true,
    faviconPath : 'pages/img/favicon.ico',
    favicon     : null
};

function requestFavicon(request, response) {
    response.writeHead(200, {'Content-Type': 'image/vnd.microsoft.icon'});
    response.end(context.favicon);
}

function requestSystemInfo(request, response) {
    // 发送 HTTP 头部 
    // HTTP 状态值: 200 : OK
    // 内容类型: text/plain
    response.writeHead(200, {'Content-Type': 'application/json;charset=UTF-8'});

    // 发送响应数
    var systemInfo = {};
    systemInfo.cpus = os.cpus();
    systemInfo.memory = {
        total: os.totalmem(),
        free: os.freemem()
    };
    response.end(JSON.stringify(systemInfo) + "\n");
}

function gotoHomePage(request, response) {
    // 发送 HTTP 头部 
    // HTTP 状态值: 200 : OK
    // 内容类型: text/plain
    response.writeHead(200, {'Content-Type': 'text/html'});

    // 发送响应数据 "Hello World"
    response.end(fs.readFileSync("pages/index.html"));
}

function gotoNoFoundPage(request, response) {
}


////////////////////////////////////////////////
// Main entry
////////////////////////////////////////////////
// Create caches
context.favicon = fs.readFileSync(context.faviconPath);

// Create http server
http.createServer(function (request, response) {

    // 解析URL
    var pathname = url.parse(request.url).pathname;
    if (context.printDebug) {
        console.log("[Request] pathname: " + pathname);
    }

    if (pathname == "/favicon.ico") {
        requestFavicon(request, response);
    } else if (pathname == "/" || pathname == "/index.html") {
        if (context.printDebug) {
            console.log("[Request] goto Home page");
        }
        gotoHomePage(request, response);
    } else if (pathname == "/SystemInfo") {
        if (context.printDebug) {
            console.log("[Request] goto System information");
        }
        requestSystemInfo(request, response);
    } else {
        if (context.printDebug) {
            console.log("[Request] goto No found page");
        }
        gotoNoFoundPage(request, response);
    }

}).listen(context.listenPort);

// 终端打印如下信息
context.address = os.networkInterfaces()['ens33'][0].address;
console.log('Server running at http://' + context.address + ':' + context.listenPort + '/');
