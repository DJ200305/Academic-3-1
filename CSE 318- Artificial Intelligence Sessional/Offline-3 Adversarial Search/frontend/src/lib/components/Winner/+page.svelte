<script>
    import {goto} from '$app/navigation';
    import { onMount } from 'svelte';
	import { on } from 'svelte/events';
    export let winner;
    export let reset;
    export let timerend;
    export let txtdeletion;
    onMount(() => {
        txtdeletion()
    });
    const Again = (winner)=>{
        timerend()
        reset();
        console.log("Resetting game");
        if (winner.length === 1){
            console.log("Winner length: ", winner.length);
            goto('/timer');
        } 
        else{
            goto('/aivsai');
        }
        
    }
</script>
<div class = "backdrop">
    <div class = "winner">
        {#if winner[0] === 'Draw'}
            <h1>It's a Draw!</h1>
        {:else if winner.length === 2}
            <h1>Winner is {winner[0]} {winner[1]}</h1>
        {:else}
             <h1>Winner is {winner[0]}</h1>    
        {/if}

        <button class = "button" on:click={()=>{Again(winner)}}>Play Again</button>
        <button class = "button" on:click={()=>{timerend();goto('/')}}>Back to Home</button>
    </div>

</div>
<style>
    .backdrop {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.3);
        display: flex;
        align-items: center;
        justify-content: center;
        z-index: 1000;
    }
    .winner {
        background-color: rgba(255, 255, 255, 0.9);
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
        text-align: center;
        font-size: 1.5em;
    }
    .button {
	  background: #ff3e00;
	  color: white;
	  border: none;
	  padding: 8px 12px;
	  border-radius: 2px;
	}
</style>    