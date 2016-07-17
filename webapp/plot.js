class Plot {
    constructor(canvas){
        this.canvas = canvas;
        this.range = {
            x:[-10,10],
            y:[-10,10] 
        };
    }

    plot(func){
        console.log(this.canvas.width);
        console.log(this.canvas.height);

        console.log(this.range);
    }   
}
