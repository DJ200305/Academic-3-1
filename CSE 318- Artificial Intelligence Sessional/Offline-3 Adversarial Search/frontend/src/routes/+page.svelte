<script>
    import { goto } from "$app/navigation";   
    const buttonClick = () => {
        goto('/timer');
    };
    const aivsai = () => {
        goto('/random');
    };
</script>
<div class = "body">
<div class = "main">
    <button class = "button" on:click={buttonClick}>Play</button>
    <button class = "button" on:click={aivsai}>AI VS AI</button>
</div>
</div>
<style>
     .body {
        
        height: 100vh; /* Full screen height */
        width: 100vw;  /* Full screen width */
        background-image: url('/chainreaction2.jpg'); 
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
  background-image: url('/chainreaction2.jpg');
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
        padding: 2rem;
         /* Optional: translucent background for better readability */
        border-radius: 12px;
    }

    .button {
        background: #ff3e00;  
        color: white;
        border: none;
        padding: 8px 12px;
        border-radius: 6px;
        margin: 5px;
        cursor: pointer;
    }
</style>    
