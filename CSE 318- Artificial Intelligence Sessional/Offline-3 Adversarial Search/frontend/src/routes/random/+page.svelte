<script>
     import {goto} from "$app/navigation";
    const handleClick = (type) => {
        if (type === "random") {
            localStorage.setItem('mode', 'random');
        } else {
            localStorage.setItem('mode', 'default');
        }
        localStorage.setItem('timer1',90)
        goto('/aivsai');
    }    
</script>

<div class = "main">
    <h1>Select how the game will be</h1>
    <button class="button" on:click={()=>handleClick("random")}>Random Move vs Calculation Based Move</button>
    <button class="button" on:click={()=>handleClick("default")}>Calculation Based Move vs Calculation Based Move</button>
</div>

<style>
    .main {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        height: 100vh;
        background-color: #f0f0f0;
    }
    .button {
      background: #ff3e00;
      color: white;
      border: none;
      padding: 8px 12px;
      border-radius: 2px;
      margin: 5px;
    }
</style>