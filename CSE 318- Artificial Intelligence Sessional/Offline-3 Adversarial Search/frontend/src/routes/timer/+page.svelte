<script>
	import { goto } from "$app/navigation";

    const timers = {
        "1": 60,
        "2": 120,
        "0": "Disabled"
    }
    const handleClick = (t)=>{
        localStorage.setItem('timer', timers[t])
        goto('/game');
    }
</script>
<div class = "body">
<div class = "main">
    <h1>Select a time</h1>
    <button class="button" on:click={()=>handleClick("1")}>1 min</button>
    <button class="button" on:click={()=>handleClick("2")}>2 min</button>
    <button class="button" on:click={()=>handleClick("0")}>None</button>
</div>
</div>
<style>
    .body {
        
        height: 100vh; /* Full screen height */
        width: 100vw;  /* Full screen width */
        background-image: url('/sandtimer.jpg'); 
        background-size: contain;
        background-repeat: no-repeat;
        background-position: center center;
        display: flex;
        justify-content: center;
        align-items: center;
    }
.body::before {
  content: "";
  position: absolute;
  top: 0; left: 0; right: 0; bottom: 0;
  background-image: url('/sandtimer.jpg');
  background-size: cover; /* covers whole area */
  background-position: center;
  filter: blur(15px);
  opacity: 0.3;
  z-index: -1; /* behind content */
}
    .main {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        height: 100vh;
        
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