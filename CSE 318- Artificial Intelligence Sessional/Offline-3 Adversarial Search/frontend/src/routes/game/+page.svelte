<script>
	import Winner from '$lib/components/Winner/+page.svelte';
	import {onMount} from 'svelte';
    let winner = null;
	let state = Array.from({length:9},()=>Array(6).fill('0'))
	let over = false
	let player = 'R'
	let turn = "Human Move:"
	let first = 0
	let timeshow = false
	let inttime = 0;
	let vtime = 0
	let interval
	let rcnt = 0
	let bcnt = 0
	onMount(async () => {
		let time = (localStorage.getItem('timer'));
		if(time === "Disabled"){
           timeshow = false
		}
		else{
			timeshow = true;
			console.log(timeshow)
			inttime = parseInt(time);
			starttime(inttime)
		}
	});
	async function starttime(inttime){
		vtime = inttime
        interval=setInterval(()=>{
			if(vtime > 0){
				vtime = vtime - 1
			}
			else{
				clearInterval(interval)
				timeshow = false
				over = true
				let cnt1 = 0,cnt2 = 0
				for(let i=0;i<9;i++){
					for(let j=0;j<6;j++){
						let cell = state[i][j]
						if(cell.length === 2){
							let [charcnt,owner] = cell.split('');
							if(owner === 'R'){
								cnt1 += parseInt(charcnt);
							}
							else if(owner === 'B'){
								cnt2 += parseInt(charcnt);
							}
						}
					}
				}
				if(cnt1 > cnt2){
					winner = ['player1']
				}
				else if(cnt2 > cnt1){
					winner = ['player2']
				}
				else{
					winner = ['Draw']
				}
			}
		},1000)
	}
	async function counterR(){
		let cnt = 0
		for(let i=0;i<9;i++){
			for(let j=0;j<6;j++){
				let cell = state[i][j];
				if(cell.length === 2 && cell[1] === 'R'){
					let [charcnt,owner] = cell.split('');
					cnt += parseInt(charcnt);
					
				}
			}
		}
		rcnt = cnt
		
	}
	async function counterB(){
		let cnt = 0
		for(let i=0;i<9;i++){
			for(let j=0;j<6;j++){
				let cell = state[i][j];
				if(cell.length === 2 && cell[1] === 'B'){
					let [charcnt,owner] = cell.split('');
					cnt += parseInt(charcnt);
					
				}
			}
		}
		bcnt = cnt
		
	}
	async function reset(){
		state = Array.from({length:9},()=>Array(6).fill('0'));
		over = false;
		player = 'R';
		turn = "Human Move:";
		winner = null;
		first = 0;
		rcnt = 0
		bcnt = 0;
		console.log("Game reset");
        
	}
	async function timerend(){
		status = "Game Over"
			let line = [status]
		for(let i=0;i<9;i++){
			line.push(state[i].join(' '));
			
		}
		line.push(`${winner[0]}\n`)
		console.log("In over")
		console.log(line.join('\n'));
		await fetch('http://127.0.0.1:8000/api/write',{ 
			method: 'POST',
			headers: {
				'Content-Type': 'application/json'
			},
			body: JSON.stringify({frontend: line.join('\n')})
		})
		return
	}
	async function handleClick(r,c){
		if(state[r][c] === '0'){
			state[r][c] = '1R';
		}
		else{
		if(over){
			return 
		}
		let cell = state[r][c]
		let [charcnt,owner] = cell.split('');
		if(owner !== player){
			console.log("Not your turn")
			console.log(owner)
			return
		}
		if(charcnt === '0'){
			state[r][c] = '1' + player;

		}
		else if(charcnt !== '0' && owner === player){
			let count = parseInt(charcnt);
			count++
			let c1 = count - '0'
			state[r][c] = (c1)+player;
		}
	}
	first++
	console.log(state);
		await explosion(r,c);
		state = state.map(row => [...row]);
        await write();
		await counterR();
		await counterB();
		const ai_move = await fetch('http://127.0.0.1:8000/move', {method: 'GET'})
		const output = await ai_move.json();
		await aimove(output.output);
		await counterR();
		await counterB();
	}
	async function write(){
		let line = [turn]
		for(let i=0;i<9;i++){
			line.push(state[i].join(' '));
			
		}
		console.log(line.join('\n'));
		await fetch('http://127.0.0.1:8000/api/write',{ 
			method: 'POST',
			headers: {
				'Content-Type': 'application/json'
			},
			body: JSON.stringify({frontend: line.join('\n')})
		})
	}
	async function aimove(output){
        status = output[0]
		if (status === 'Game Over'){
			over = true
			winner = [output[10]]
			clearInterval(interval)
			return 
		}
		for(let i=0;i<9;i++){
			state[i] = output[i+1].split(' ')
		}
	}
    function cmass(r,c){
		if (r === 0 && c === 0 || r === 0 && c === 5 || r === 8 && c === 0 || r === 8 && c === 5 ){
			return 2;
		}
		else if ((c === 0 && r !== 0) || (c === 5 && r !== 0) || (r === 0 && c !== 0) || (r === 8 && c !== 0)){
			return 3;
		}
		else{
			return 4;
		}

	}
    async function explosion(r,c){
		console.log("Explosion at:", r, c);
        let q = [[r,c]];
		let cross = cmass(r,c);
		console.log("Printing cross:");
		console.log(cross);

		let cell = state[r][c];
		let count
		if(cell === '0'){
			count = 0;
		}
		else{
		let [charcnt,owner] = cell.split('');
		 count = parseInt(charcnt);
	}
	    console.log("Printing count:")
		console.log(count);
		if(count < cross){
			return state
		}
		while(q.length > 0){
			let pair = q.shift();
			let x = pair[0];
			let y = pair[1];
			let dir = [[-1,0],[1,0],[0,-1],[0,1]];
			for(let i=0;i<4;i++){
				let nx = x + dir[i][0];
				let ny = y + dir[i][1];
				if(nx >= 0 && nx < 9 && ny >= 0 && ny < 6){
                    let [charcnt,charowner] = state[nx][ny].split('');
					let cnt = parseInt(charcnt);
					cnt++
					state[nx][ny]= `${cnt}${state[x][y].split('')[1]}`
					let owner = state[nx][ny][1];
                    if(cnt >= cmass(nx,ny)){
						q.push([nx,ny]);
					}
				}
			}
			state[x][y] = '0';
		}
		return state
	}

</script>


{#if winner}
    <Winner winner = {winner} reset = {reset} timerend = {timerend}/>
{:else}
<div class = "wrapper">
<div class = "main">
    <div class="state">
       
			{#each state as row,r}
				
					{#each row as cell,c}
		
						<button class = "cell" on:click={()=>{handleClick(r,c)}}>
							{#if cell !== '0'}
							    {#each Array(parseInt(cell[0])) as _,i}
								   <span class = "orb {cell[1]}"></span>
								{/each}
							{/if}	   
						</button>
					{/each}
				
			{/each}
		
	</div>	
	<div class = "counter">
        <div class = "orb R"></div><span>{rcnt}</span>
		<div class = "orb B"></div><span>{bcnt}</span>
	</div>
</div>	
</div>
{/if}

{#if timeshow}
	<div class="timer">
		⏳ {Math.floor(vtime / 60)}:{(vtime % 60).toString().padStart(2, '0')}
	</div>
{/if}

<style>
	.main{
		display: flex;
	flex-direction: row;
	align-items: center;
	justify-content: center;
	gap: 2em;
	}
	.orb {
	width: 10px;
	height: 10px;
	border-radius: 50%;
	display: inline-block;
	margin: 1px;
}
	.orb.R {
		background-color: red;
	}
	.orb.B {
		background-color: blue;
	}
	.counter {
	display: flex;
	flex-direction: column;
	justify-content: center;
	align-items: flex-start;
	font-size: 1.2em;
	background: white;
	padding: 1em;
	border-radius: 12px;
	box-shadow: 0 4px 12px rgba(0,0,0,0.1);
	gap: 1em;
}
.counter .orb {
	width: 16px;
	height: 16px;
	margin-right: 8px;
	display: inline-block;
}

.counter span {
	vertical-align: middle;
	margin-left: 0.5em;
}
	.wrapper {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
		height: 100vh;
		background-color: #f0f0f0;
	}
	.state {
		display: grid;
		grid-template-columns: repeat(6, 40px);
		gap: 5px;
		margin-top: 2em;
		text-align: center;
	}
	.cell {
		width: 40px;
		height: 40px;
		border: 1px solid #ccc;
		display: flex;
		align-items: center;
		justify-content: center;
		font-size: 1.5em;
		cursor: pointer;
	}
	.timer{
		position: fixed;
    top: 1rem;
    right: 1rem;
    background: #222;
    color: #fff;
    padding: 0.5rem 1rem;
    border-radius: 0.5rem;
    font-size: 1.5rem;
    font-family: monospace;
    z-index: 999;
	}
</style>