#!/usr/bin/bash

base="$(cd "$(dirname "$0")" && pwd)"
argnum=$#
if [[ $argnum < 4 ]]; then
    echo "Usage: You must provide 4 mandatory arguments: <path to submission folder> <path to target folder> <path to test folder> <path to answer folder>"
    exit 1
fi
submission=$1
target=$2
test=$3
answer=$4

lc=true
cc=true
fc=true
ex=true
verbose=false
for args in "$@"; do
    if [[ "$args" == "-nolc" ]]; then 
        lc=false
    elif [[ "$args" == "-v" ]]; then
        verbose=true    
    elif [[ "$args" == "-nocc" ]]; then 
        cc=false
    elif [[ "$args" == "-nofc" ]]; then 
        fc=false
    elif [[ "$args" == "-noexecute" ]]; then 
        ex=false
    fi
done

declare -A arr

mkdir -p targets
cd targets
mkdir -p C C++ Python Java
cd ..

output="$base/output.csv"
if [[ ! -f "$output" ]]; then
    echo "student_id,student_name" > "$output"
fi


moving() {
    # echo "in2"
    cd $1
    for f in *.zip; do
        # echo "in3"
        [[ ! -f "$f" ]] &&  continue
        # echo "in1"
        filename=$(basename "$f" .zip)
        IFS='_' read -r -a parts <<< "$filename"
        name="${parts[0]}"
        roll="${parts[-1]}"
        if [[ "$verbose" == true && "$ex" == true ]]; then
            echo "Organizing files of $roll"
            echo "Executing files of $roll"
        elif [[ "$verbose" == true ]]; then
            echo "Organizing files of $roll"
        fi    
        if ! grep -q "$name,$roll" "$output"; then
            echo "$roll,$name" >> "$output"
        fi 

        if [[ ! -d "$filename" ]]; then
            # echo "in"
            unzip  -q "$f" 
        fi

        
        
        find "$filename" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \) | while read -r file; do
            # echo "in4"
            dest="/$base/$2"
            ext="${file##*.}"
            
            if [[ "$ext" == "cpp" ]]; then
                mkdir -p "$dest/C++/$roll"
                mv -n "$file" "$dest/C++/$roll/"
            elif [[ "$ext" == "c" ]]; then
                mkdir -p "$dest/C/$roll"
                mv -n "$file" "$dest/C/$roll/"
            elif [[ "$ext" == "java" ]]; then
                mkdir -p "$dest/Java/$roll"
                mv -n "$file" "$dest/Java/$roll/"
            elif [[ "$ext" == "py" ]]; then
                mkdir -p "$dest/Python/$roll"
                mv -n "$file" "$dest/Python/$roll/"
            fi
        done
    done  
    cd ..  
    echo "All submissions processed successfully."
}


renaming() {
    find "$target" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \) | while read -r file; do
        name=$(basename "$file")
        ext="${file##*.}"
        bname="${name%.*}"
        if [[ "$bname" != "main" && "$ext" == "c" ]]; then
            mv -n "$file" "$(dirname "$file")/main.c"
        elif [[ "$bname" != "main" && "$ext" == "cpp" ]]; then
            mv -n "$file" "$(dirname "$file")/main.cpp"
        elif [[ "$bname" != "main" && "$ext" == "java" ]]; then
            mv -n "$file" "$(dirname "$file")/Main.java"
        elif [[ "$bname" != "main" && "$ext" == "py" ]]; then
            mv -n "$file" "$(dirname "$file")/main.py"
        fi
    done    
}

moving "$submission" "$target"
renaming


declare -A namemp


declare -A lang
declare -A line
declare -A comments
declare -A functions
declare -A matches
declare -A unmatches


  result_file="result.csv"


line_count() {
    
    code_metrics+=("line_count")
    while IFS= read -r file; do
        lines=$(wc -l < "$file")
        name=$(basename "$file")
        ext="${file##*.}"
        bname="${name%.*}"
        roll=$(basename "$(dirname "$file")")

        
        arr["$roll,line_count"]=$lines
        
    done < <(find "$target" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \))   
}


comments() {
    
    code_metrics+=("comment_count")
     while IFS= read -r file; do
        lines=$(grep -c '//' "$file")
        name=$(basename "$file")
        ext="${file##*.}"
        bname="${name%.*}"
        roll=$(basename "$(dirname "$file")")
         
        if [[ "$ext" == "c" || "$ext" == "cpp" || "$ext" == "java" ]]; then
            cnt=$(grep -c '//' "$file")
            arr["$roll,comment_count"]=$cnt
        elif [[ "$ext" == "py" ]]; then
            cnt=$(grep -c '#' "$file")
            arr["$roll,comment_count"]=$cnt
           
        fi
        
    done < <(find "$target" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \))  
}


func_cnt() {
    
    code_metrics+=("function_count")
    while IFS= read -r file; do
        lines=$(grep -c '//' "$file")
        name=$(basename "$file")
        ext="${file##*.}"
        bname="${name%.*}"
        roll=$(basename "$(dirname "$file")")
         
        if [[ "$ext" == "c" || "$ext" == "cpp" ]]; then
            cnt=$(grep -E -c '^[a-zA-Z_][a-zA-Z0-9_ \*\t]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\(.*\)\s*\{' "$file")
            arr["$roll,function_count"]=$cnt
        elif [[ "$ext" == "java" ]]; then
            cnt=$(grep -c "[a-zA-Z_][a-zA-Z0-9_]* [a-zA-Z_][a-zA-Z0-9_]* [a-zA-Z_][a-zA-Z0-9_]*(.*)" "$file")
            arr["$roll,function_count"]=$cnt 
        elif [[ "$ext" == "py" ]]; then
            cnt=$(grep -E -c '^\s*def\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\(.*\)\s*:' "$file")
            arr["$roll,function_count"]=$cnt    
        fi
        
    done < <(find "$target" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \))
}

exec_match() {
    
    code_metrics+=("matched")
    code_metrics+=("not_matched")
    while IFS= read -r file; do
        ext="${file##*.}"
        roll=$(basename "$(dirname "$file")")
        dir="$(dirname "$file")"
        if [[ "$ext" == "c" || "$ext" == "cpp" ]]; then
           g++ "$file" -o "$dir/main.out"
           run="$dir/main.out"
        
        fi
        match=0
        unmatch=0
        for t in "$test"/*.txt; do
           num=$(basename "$t" | grep -o '[0-9]*')
           out_dir="$dir/out${num}.txt"
        
           if [[ "$ext" == "java" ]];then
                javac "$file"
                java -cp "$dir" Main < "$t" > "$out_dir"
           elif [[ "$ext" == "c" || "$ext" == "cpp" ]]; then
                "$run" < "$t" > "$out_dir"   
           elif [[ "$ext" == "py" ]]; then
                python3 "$file" < "$t" > "$out_dir"
           fi    

           if diff -q "$out_dir" "$answer/ans${num}.txt" > /dev/null; then
                ((match++))
           else
                ((unmatch++))
            fi
        done
       
        arr["$roll,matched"]=$match
        arr["$roll,not_matched"]=$unmatch
    done < <(find "$target" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \))     
 }
#exec_match "$target" "$test" "$answer"
lang_check() {
    
    code_metrics+=("language")
    while IFS= read -r file; do
        name=$(basename "$file")
        ext="${file##*.}"
        bname="${name%.*}"
        roll=$(basename "$(dirname "$file")")
        if [[ "$ext" == "c" ]]; then
            arr["$roll,language"]="C"
        elif [[ "$ext" == "cpp" ]]; then
            arr["$roll,language"]="C++"
        elif [[ "$ext" == "java" ]]; then
            arr["$roll,language"]="Java"
        elif [[ "$ext" == "py" ]]; then
            arr["$roll,language"]="Python"
        fi
        
    done < <(find "$target" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.java" -o -iname "*.py" \))   
}

naming() {
    name_file="output.csv"
 result_file="result.csv"
if [[ -f "$name_file" ]]; then
    while IFS=',' read -r roll name; do
        [[ $roll == "student_id" ]] && continue  
        namemp["$roll"]=$name
    done < "$name_file"
fi
}

lang_check 

if [[ "$ex" == true ]]; then
    exec_match
fi

if [[ "$lc" == true ]]; then
    line_count
fi
if [[ "$cc" == true ]]; then
    comments
fi
if [[ "$fc" == true ]]; then
    func_cnt
fi



naming



            
csv() {
    header="student_id,student_name"
for metric in "${code_metrics[@]}"; do
    header+=",$metric"
done
> "$result_file" 
echo "$header" >> "$result_file"

for roll in "${!namemp[@]}"; do
    line="$roll,${namemp[$roll]}"
    for metric in "${code_metrics[@]}"; do
        
        val=${arr["$roll,$metric"]}
        line+=",${val}"  
    done
    
    echo "$line" >> "$result_file"
done
tmp_file=$(mktemp)
    { head -n 1 "$result_file"; tail -n +2 "$result_file" | sort -t',' -k2; } > "$tmp_file"
    mv "$tmp_file" "$result_file"
}

csv

mv "$result_file" "$target"
