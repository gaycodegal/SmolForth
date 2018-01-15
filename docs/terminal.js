function Terminal(div){
    div.innerHTML = "";
    this.container = div;
    this.history = 1000;
    this.display = 1000;
    this.currentline = [];
}

Terminal.prototype.writeline = function(text){
    var line = document.createElement("div");
    line.textContent = text;
    this.container.appendChild(line);
};

Terminal.prototype.write = function(text){
    var lines = text.split("\n");
    for(var i = 0; i < lines.length; ++i){
	if(this.currentline.length){
	    this.writeline(this.currentline.join(""));
	    this.currentline = [];
	}
	this.currentline.push(lines[i]);
    }
    //this.temp.textContent = "";
};

var terminal;

function init(){
    terminal = new Terminal(document.getElementById("Terminal"));
    document.body.appendChild(terminal.container);
    terminal.write("hi\nyou!");
}
