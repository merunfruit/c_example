"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {
	var errorTimer = setTimeout(function () {error()}, 10000);
	window.setInterval(function() {updateInfo()}, 1000);
	

	$('#modeNone').click(function(){
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("track 0");
	});
	$('#modeRock').click(function(){
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("track 1");
	});
	$('#modeCustom').click(function(){
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("track 2");
	});
	$('#volumeDown').click(function(){
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("volume -5");
	});
	$('#volumeUp').click(function() {
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("volume +5");
	});
	$('#bpmDown').click(function() {
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("bpm -5");
	});
	$('#bpmUp').click(function() {
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("bpm +5");
	});
	$('#snare').click(function() {
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("sound 2");
	});
	$('#hihat').click(function() {
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("sound 1");
	});
	$('#base').click(function() {
		errorTimer = setTimeout(function () {error()}, 10000);
		sendBeatboxCommand("sound 0");
	});
	
	
	socket.on('commandReply', function(result) {
		clearTimeout(errorTimer);
		$('#error-box').hide();
		var str = result;
		var res = str.split(" ");
		if(res[0] == "volume") {
			$('#volumeid').val(res[1]);
		}
		if(res[0] == "bpm") {
			$('#bpmid').val(res[1]);
		}
		if(res[0] == "Uptime") {
			$('#status').text(str);
		}
		if(res[0] == "mode") {
			$('#modeid').text(res[1]);
		}
	});
	
});

function error() {
	$('#error-text').text("Lost connection to C program");
	$('#error-box').show();
}

function updateInfo() {
	sendBeatboxCommand("volume 0");
	sendBeatboxCommand("bpm 0");
	sendBeatboxCommand("uptime");
	sendBeatboxCommand("mode");
}

function sendBeatboxCommand(message, errorTimer) {
	socket.emit('beatbox', message);
};