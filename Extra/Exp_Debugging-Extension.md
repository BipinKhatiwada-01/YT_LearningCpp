# VSCodium + GDB Debugger Incorrect Variable Value — Full Investigation Report

 ## Executive Summary

 A C++ program being debugged in **VSCodium** appeared to contain an incorrect value for a local variable.

 The program accepted the input:

```
5
```

 and behaved correctly when run normally and when debugged using **plain GDB from a terminal**.

 However, when debugged through VSCodium, GDB reported:

```
p x
$1 = 10
```

 even though `x` had actually received `5`.

 The investigation initially considered several possible causes, including:

 - `std::cin`
- the `if` condition
- uninitialized variables
- stale source code
- an old executable
- compiler problems
- incorrect debugger input handling
- GDB itself
- GDB/MI configuration
- variable caching

 The decisive discovery was that **two different C++ debugging extensions were installed in VSCodium**:

 1. **Native Debug**
   - Publisher: WebFreak
   - Version: `0.27.0`
   - Extension ID: `webfreak.debug`
   - Uses debugger configuration type:

     ```
     "type": "gdb"
     ```
2. **Kylin C/C++ Debug**
   - Publisher/team: Kylin IDE Team
   - Version: `0.4.0`
   - Extension ID:

     ```
     kylinideteam.cppdebug
     ```
   - Uses:

     ```
     "type": "cppdbg"
     ```

 The original `launch.json` used:

```
"type": "gdb"
```

 which meant the debugging session was being handled by **WebFreak Native Debug**.

 After switching the configuration to:

```
"type": "cppdbg"
```

 the Kylin C/C++ debugger was used instead.

 The exact same program then correctly reported:

```
x = 5
```

 Therefore, the practical root cause was **the WebFreak Native Debug integration/configuration**, not the C++ program, compiler, executable, `std::cin`, or ordinary GDB execution.

---

 # 1\. Original Problem

 The original C++ program contained:

```
#include <iostream>

int main(){

    int x = 0;
    std::cout<<"Enter a value for x : ";
    std::cin>>x;
    bool CheckResult  = x == 5;

    if( CheckResult){
        std::cout<<"Hello, World"<<std::endl;
    }

    std::cout<<"Hi lol"<<std::endl;

    const char* ptr = "Hi";

    if(ptr != nullptr){
        std::cout<<"Pointer is NOT null"<<std::endl;
    }

    if(ptr == "Hi"){
        std::cout<<"Pointer says 'Hi'"<<std::endl;
    }
    else if(ptr != "Hi"){
        std::cout<<"Pointer is NOT saying 'Hi'"<<std::endl;
    }
    else{
        std::cout<<"Cricket Stridulations"<<std::endl;
    }
}
```

 The relevant part was:

```
int x = 0;

std::cin >> x;

bool CheckResult = x == 5;
```

 The expected behavior was:

```
Input: 5
x: 5
x == 5: true
```

 and therefore:

```
Hello, World
```

 should be printed.

---

 # 2\. Initial Symptom

 When debugging through VSCodium, the debugger appeared to report:

```
x = 10
```

 Consequently:

```
x == 5
```

 appeared to evaluate to:

```
false
```

 and:

```
if (CheckResult)
```

 was not entered.

 This was confusing because the program visibly received the input:

```
5
```

 and the executable worked correctly when run normally.

---

 # 3\. Initial GDB Input Confusion

 At the beginning of the investigation, a breakpoint was placed around:

```
std::cout << "Enter a value for x : ";
std::cin >> x;
```

 The debugger showed something similar to:

```
Breakpoint 1, main () ... line 6
6 std::cout<<"Enter a value for x : ";
5

Undefined command: "5".
```

 This initially suggested that `5` had been entered into the **GDB command interface** instead of the program's standard input.

 That was a legitimate issue to investigate, but it was ultimately **not the root cause**.

 Later testing demonstrated that the program was in fact receiving `5`.

---

 # 4\. Configuration Initially Used

 The VSCodium configuration was:

```
{
  "version": "0.2.0",

  "configurations": [

    {
      "type": "gdb",
      "request": "launch",
      "name": "Debug (type path)",
      "target": "${workspaceFolder}/${input:binaryPath}",
      "cwd": "${workspaceFolder}",
      "valuesFormatting": "prettyPrinters"
    }

  ],

  "inputs": [

    {
      "id": "binaryPath",
      "type": "promptString",
      "description": "Path to binary, relative to workspace root"
    }

  ]
}
```

 An attempt was made to add:

```
"externalConsole": true
```

 but the debugger reported that the property was not allowed.

 This was an early indication that this was **not Microsoft's normal `cppdbg` configuration**.

---

 # 5\. GDB Warning

 Every VSCodium debugging session also produced:

```
Warning: 'set target-async', an alias for the command 'set mi-async', is deprecated.

Use 'set mi-async'.
```

 This was investigated as a possible clue.

 The warning was ultimately treated as **secondary**, rather than the direct cause of the incorrect variable value.

 It did, however, reinforce that the VSCodium debugger was using a particular GDB/MI integration layer.

---

 # 6\. Simplifying the Program

 To eliminate the rest of the original program as a source of the problem, the program was reduced to:

```
#include <iostream>

int main() {
    int x = 123456;

    std::cout << "Before cin: " << x << '\n';

    std::cin >> x;

    std::cout << "After cin: " << x << '\n';

    return 0;
}
```

 This was an important diagnostic step.

 The debugger was stopped after:

```
std::cin >> x;
```

 The user entered:

```
5
```

 VSCodium then reported:

```
p x

$1 = 10
```

 This established that the problem had nothing to do with:

```
bool CheckResult = x == 5;
```

 or the later pointer/string experiments.

 The discrepancy existed in a minimal program containing essentially:

```
int x = 123456;
std::cin >> x;
```

---

 # 7\. Address Investigation

 The debugger also reported:

```
p &x

$2 = (int *) 0x7fffffffdc0c
```

 Additional investigation using:

```
x/wd &x
```

 also produced:

```
10
```

 This was significant because it demonstrated that the issue was not merely the VSCodium Variables panel displaying a cached value.

 The GDB session being driven by the problematic debugger integration was actually observing `10` at the address associated with `x`.

---

 # 8\. Other Debugger Checks

 The debugger was queried with:

```
p CheckResult
```

 which produced:

```
$3 = false
```

 This was consistent with the debugger's incorrect interpretation that:

```
x = 10
```

 because:

```
10 == 5
```

 is false.

 The source listing was also checked with:

```
list
```

 and showed the current source.

 For example:

```
3 int main(){
5    int x = 123456;
6    std::cout<<"Enter a value for x : ";
7    std::cin>>x;
8    bool CheckResult = x == 5;
```

 Therefore the debugger was not simply displaying an obviously stale source listing.

---

 # 9\. Executable Verification

 The debugger was also checked with:

```
info files
```

 This showed the expected executable.

 This ruled against the simple explanation:

 > "VSCodium is obviously debugging an old executable."

 The executable/source association appeared correct.

---

 # 10\. Plain GDB Control Experiment

 This was the most important control experiment.

 The executable was compiled with debugging information:

```
g++ -g
```

 The same executable was then launched directly through **plain GDB in a terminal**, without VSCodium.

 Initially, there was a misleading result.

 GDB stopped at:

```
int x = 123456;
```

 and:

```
print x
```

 returned:

```
$2 = 32767
```

 This was correctly identified as normal because the debugger was stopped **before the initialization statement had executed**.

 After:

```
next
```

 the debugger moved to:

```
std::cout << "Before cin: " << x << '\n';
```

 and:

```
print x
```

 returned:

```
$3 = 123456
```

 This proved that the initialization was correct.

 The program was then allowed to reach:

```
std::cin >> x;
```

 The input:

```
5
```

 was supplied.

 The program printed:

```
After cin: 5
```

 and exited normally.

 Therefore plain GDB successfully demonstrated:

```
initial x = 123456
input = 5
final x = 5
```

---

 # 11\. What This Ruled Out

 At this point, several explanations were effectively eliminated.

 ## C++ logic

 The expression:

```
x == 5
```

 is correct C++.

 There is nothing inherently wrong with the `if`.

 ## `std::cin`

 Plain GDB demonstrated:

```
5
After cin: 5
```

 so `std::cin` correctly stored the input.

 ## Compiler

 The executable behaved correctly under plain GDB.

 ## Executable

 The expected executable was being debugged.

 ## Source

 The current source was visible in GDB.

 ## Uninitialized variable

 At the relevant breakpoint, execution had already passed:

```
std::cin >> x;
```

 and plain GDB demonstrated correct initialization and input.

 ## Pointer/string code

 The later:

```
const char* ptr = "Hi";
```

 code was completely unrelated.

---

 # 12\. Discovery of the Two Debugger Extensions

 The crucial discovery came from examining the installed VSCodium extensions.

 There were **two C/C++ debugging extensions** installed.

 ### Extension 1 — WebFreak

```
Name: Native Debug
Publisher: webfreak
Version: 0.27.0
Extension ID: webfreak.debug
```

 This extension provides the debugger configuration:

```
"type": "gdb"
```

 Therefore the original configuration:

```
"type": "gdb"
```

 was using **WebFreak Native Debug**.

---

 ### Extension 2 — Kylin

```
Name: Kylin C/C++ Debug
Identifier: kylinideteam.cppdebug
Version: 0.4.0
```

 This extension uses the debugger configuration type:

```
"type": "cppdbg"
```

 This was the critical distinction.

---

 # 13\. The Actual Configuration Problem

 The original configuration was:

```
"type": "gdb"
```

 That meant:

```
VSCodium
   ↓
WebFreak Native Debug
   ↓
GDB/MI
   ↓
Program
```

 The problematic behavior occurred in that setup.

 The alternative debugger was:

```
Kylin C/C++ Debug
```

 which uses:

```
"type": "cppdbg"
```

 Therefore the fix was not to change the C++ program.

 It was to change **which debugger extension handled the debugging session**.

---

 # 14\. Final Fix

 The debugger configuration was changed from:

```
"type": "gdb"
```

 to:

```
"type": "cppdbg"
```

 A corresponding `cppdbg` configuration was used with GDB.

 For example:

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug C++",
            "type": "cppdbg",
            "request": "launch",

            "program": "${workspaceFolder}/${input:binaryPath}",

            "cwd": "${workspaceFolder}",

            "MIMode": "gdb",

            "miDebuggerPath": "/usr/bin/gdb",

            "stopAtEntry": false,

            "externalConsole": false
        }
    ],

    "inputs": [
        {
            "id": "binaryPath",
            "type": "promptString",
            "description": "Path to binary, relative to workspace root"
        }
    ]
}
```

 The important change was:

```
- "type": "gdb"
+ "type": "cppdbg"
```

---

 # 15\. Final Result

 After switching to the Kylin `cppdbg` debugger:

```
x = 5
```

 was correctly reported.

 🎉

 The same source and executable now produced the expected debugger value.

 Therefore the original discrepancy disappeared without changing the C++ program.

---

 # 16\. Final Diagnosis

 The practical diagnosis is:

 > **The incorrect `x = 10` value was caused by the VSCodium debugging integration being used by the `webfreak.debug` Native Debug extension (`v0.27.0`).**

 The program itself was correct.

 The compiler was correct.

 The executable was correct.

 Plain GDB was correct.

 The problem was specific to the debugger integration selected by:

```
"type": "gdb"
```

 Switching to the other installed debugger:

```
"type": "cppdbg"
```

 provided by:

```
Kylin C/C++ Debug
kylinideteam.cppdebug
v0.4.0
```

 resolved the problem.

 ### Important qualification

 This investigation establishes the **practical fault boundary**, but it does **not** establish exactly which internal bug in WebFreak Native Debug caused `10` to appear. We did not isolate a specific line of its source code or a particular GDB/MI packet responsible for the incorrect state.

 So the safest technical wording is:

 > **The incorrect value was specific to the WebFreak Native Debug (`webfreak.debug` v0.27.0) debugging path. Switching to Kylin C/C++ Debug (`kylinideteam.cppdebug` v0.4.0) using `cppdbg` fixed the issue.**

 Rather than:

 > "GDB itself has a bug."

 We have evidence against that, because plain GDB worked correctly.

---

 # 17\. Final Setup

 The working architecture is now:

```
             C++ source
                 │
                 ▼
             g++ -g
                 │
                 ▼
             executable
                 │
                 ▼
               GDB
                 │
                 ▼
       Kylin C/C++ Debug
       (kylinideteam.cppdebug)
                 │
                 ▼
              VSCodium
```

 The broken path was:

```
             C++ source
                 │
                 ▼
             g++ -g
                 │
                 ▼
             executable
                 │
                 ▼
               GDB
                 │
                 ▼
        WebFreak Native Debug
          (webfreak.debug)
                 │
                 ▼
              VSCodium
```

---

 # 18\. Lessons Learned

 ### 1\. A debugger can be wrong even when the program is right

 A debugger is another piece of software sitting between you and the program.

 If the program behaves correctly but the debugger reports impossible state, the debugger integration itself can be the problem.

 ### 2\. Always perform a plain-GDB control experiment

 Running:

```
gdb ./program
```

 was extremely useful here.

 It established that:

```
compiler → executable → GDB
```

 was functioning correctly.

 That narrowed the problem to:

```
VSCodium → debugger extension → GDB
```

 ### 3\. Debugger `type` matters

 These are not interchangeable:

```
"type": "gdb"
```

 and:

```
"type": "cppdbg"
```

 They can correspond to **different debugger extensions**.

 ### 4\. Multiple debugger extensions can cause confusion

 Having both:

```
webfreak.debug
```

 and:

```
kylinideteam.cppdebug
```

 installed meant that VSCodium had multiple debugging implementations available.

 The `type` in `launch.json` determines which configuration provider handles the session.

 ### 5\. Don't immediately blame the C++ code

 The original program contained several things that could have distracted the investigation:

```
std::cin
bool CheckResult
const char*
string literals
pointer comparison
```

 But reducing the program to:

```
int x = 123456;
std::cin >> x;
```

 showed that the problem persisted.

 ### 6\. The weird value `10` was a symptom, not a C++ mystery

 There was no C++ rule that could legitimately turn:

```
input = 5
```

 into:

```
x = 10
```

 The fact that plain GDB and normal execution both produced `5` was the key evidence.

---

 # 19\. Minimal Reproduction for Future Reference

 If this ever happens again, the fastest reproduction is:

```
#include <iostream>

int main()
{
    int x = 123456;

    std::cout << "Before cin: " << x << '\n';

    std::cin >> x;

    std::cout << "After cin: " << x << '\n';

    return 0;
}
```

 Compile:

```
g++ -g -O0 test.cpp -o test
```

 Run normally:

```
./test
```

 Run with plain GDB:

```
gdb ./test
```

 Then compare with the VSCodium debugger.

 If plain GDB says:

```
x = 5
```

 but a VSCodium adapter says:

```
x = 10
```

 check the installed debugger extensions **before spending hours investigating the C++ code**.

---

 # Bottom Line

 **The C++ code was innocent. GDB itself was innocent.**

 The problem was the **WebFreak Native Debug adapter selected by `"type": "gdb"`**.

 You had another debugger installed:

```
Kylin C/C++ Debug
kylinideteam.cppdebug
```

 which uses:

```
"type": "cppdbg"
```

 Switching to that debugger fixed the problem:

```
BEFORE:
VSCodium → webfreak.debug → GDB → x = 10 ❌

AFTER:
VSCodium → kylinideteam.cppdebug → GDB → x = 5 ✅
```

 **2 hours of debugging something else and Case closed.** 😂