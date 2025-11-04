from fastapi import FastAPI, HTTPException,Query
import subprocess
import sys
import pydantic
from fastapi.middleware.cors import CORSMiddleware
import os
python_cmd = sys.executable  # Get the current Python executable path
app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Allow all origins
    allow_credentials=True,
    allow_methods=["*"],  # Allow all methods
    allow_headers=["*"],  # Allow all headers
)
class input(pydantic.BaseModel):
    frontend: str
class timeai(pydantic.BaseModel):
    aitime: float
@app.post("/api/logtime")
def logtime(time:timeai):
    try:
        with open("time.txt","a") as f:
            f.write(f"{time.aitime}\n")    
        return {"status": "success", "message": "Time logged successfully."}    
    except Exception as e:
         raise HTTPException(status_code=500, detail=f"Error writing to file: {str(e)}")       
@app.post("/api/write")
def write(front:input):
    try:
        with open("input.txt", "w") as f: 
            f.write(front.frontend) 
        return {"status": "success", "message": "Input written successfully."}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Error writing to file: {str(e)}")
  
@app.get("/move")
def get_move():
    try:
        subprocess.run([python_cmd, "main.py"], check=True) 
        with open("input.txt", "r") as f:
            output = f.read().splitlines()
        return {"status": "success", "output": output}
    
    except subprocess.CalledProcessError as e:
        return {"status": "error", "message": "Something went wrong with the game logic.", "error": str(e)}
    

    
@app.get("/api/ai_move")    
def get_ai1_move(mode:str = Query("default",enum=["default", "random"])):
    try:
        if mode == "random":
            subprocess.run([python_cmd, "random_ai_main.py"], check=True) 
        else:
            subprocess.run([python_cmd, "ai_main_1.py"], check=True)
        with open("input.txt", "r") as f:
            output = f.read().splitlines()
        return {"status": "success", "output": output}
    
    except subprocess.CalledProcessError as e:
        return {"status": "error", "message": "Something went wrong with the AI logic.", "error": str(e)}    

@app.get("/api/ai2_move")
def get_ai2_move():
    try:
        subprocess.run([python_cmd, "ai_main_2.py"], check=True) 
        with open("input.txt", "r") as f:
            output = f.read().splitlines()
        return {"status": "success", "output": output}
    
    except subprocess.CalledProcessError as e:
        return {"status": "error", "message": "Something went wrong with the AI logic.", "error": str(e)}    

@app.delete('/api/delete-file')
def delete_file():
    try:
        if os.path.exists("moves.txt"):
            os.remove("moves.txt")
        if os.path.exists("moves1.txt"):
            os.remove("moves1.txt")
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Error deleting files: {str(e)}")            