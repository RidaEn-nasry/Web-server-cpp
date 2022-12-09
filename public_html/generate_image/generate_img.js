

window.requestAnimFrame = (function() {
    return window.requestAnimationFrame ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame ||
        window.oRequestAnimationFrame ||
        window.msRequestAnimationFrame ||
        function(callback) {
            window.setTimeout(callback, 1000 / 60);
        };
})();
window.onload = ()=>{
    let $search = document.querySelector('.search');
    let $root = document.querySelector('.root');
    let $curveOpen = document.querySelector('.curve-open');
    let $curveClose = document.querySelector('.curve-close');
    let stateOpen = {topPointX:680,
        bottomPointY:60,
        curvePointX:690,
        curvePointY:50};
    let propsOpen = {topPointX:-150,
        bottomPointY:500,
        curvePointX:-150,
        curvePointY:450};
    let stateClose = {
        leftPointY:405,
        rightPointY:405,
        curvePointY:500
    };
    let propsClose = {
        leftPointY:0,
        rightPointY:0,
        curvePointY:0
    };
    let drawClose = ({leftPointY,rightPointY,curvePointY}) =>{
        $curveClose.setAttribute('d',`M0,${leftPointY} L0,405 740,405 740,${rightPointY} C370,${curvePointY} 370,${curvePointY} 0,${leftPointY}`)
    };
    let drawOpen = ({topPointX, bottomPointY, curvePointX, curvePointY})=>{
        $curveOpen.setAttribute('d',`M740,${bottomPointY} L740,0 ${topPointX},0 C${curvePointX},${curvePointY} ${curvePointX},${curvePointY} 740,${bottomPointY}`)
    };
    $search.addEventListener('click',()=>{
        let classes = $root.classList;
        if(classes.contains('search-open') && classes.contains('search-close')){
            classes.remove('search-close');
            animate(300, drawOpen, stateOpen, propsOpen);
        }else if(classes.contains('search-open')){
            classes.add('search-close');
            animate(300, drawClose, stateClose, propsClose);
        }else{
            classes.add('search-open');
            animate(300, drawOpen, stateOpen, propsOpen);
        }
    });
    let animate = (time, draw, state, props) =>{
        let fCount = 1;
        let start = performance.now();
        let framesCount = time/(1000 / 60);
        let keys = Object.keys(props);
        let deltaState = {};
        let stepState = {};
        keys.map((i)=>{
            (state[i] < props[i])?
                deltaState[i] = (props[i] - state[i])
                :deltaState[i] = -(state[i] - props[i])
        });
        let _animate = (timestamp) =>{
            if((timestamp - start) < time){
                keys.map((j)=>{
                    stepState[j] = state[j] + (deltaState[j]*(fCount/framesCount))
                });
                draw(stepState);
                fCount++;
                requestAnimFrame(_animate)
            }else{
                if(fCount <= framesCount){
                    draw(props);
                }
            }
        };
        requestAnimFrame(_animate);
    }
};


// input.addEventListener("keyup", function(event) {
	// 	if (event.keyCode === 13) {
		// 		console.log("key pressed");
		// 		event.preventDefault();
		// 	}
		// });
		
const input = document.getElementById("search_input");


input.addEventListener("keyup", function(event) {

	if (event.keyCode === 13) {
		event.preventDefault();
		const userInput = input.value;
		const formData = new FormData();
		formData.append('user-input', userInput);
		const url = `${window.location.origin}/cgi-bin/generate-image.js`;
    	fetch(url, {
    	    method: 'POST',
    	    body: formData
    	}).then(response => {
			if (response.status === 200) {
				response.text().then(url => {
                    window.open(url, '_blank');
    	        });
    	    } else {
				console.log("error");
    	    }
    	})
}
});
