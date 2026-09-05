
<a target="_self" title="CLICK HERE to ENTER the GATEWAY FREE!" href="https://mercwar.github.io/Constellation/index.html">
<img 
    src="https://raw.githubusercontent.com/mercwar/Robo-Knight-Gallery/refs/heads/main/Version%207/image_d2a07390.png" 
    alt="Mercwar Constellation" 
    style="width:100%; height:auto; border-radius:12px; box-shadow:0 6px 16px rgba(0,0,0,0.6);"
/>
</a>

---


# ✨ CRON 💎🔥

<h1> Command Line Editor & Graphical Interface Builder</h1>
<p>
An optimized, multi-tier Windows runtime utility pipeline that indexes local system tools, internal batch utilities, and <code>System32</code> binaries, transforming them into a unified catalog. It features an integrated database parser and provides a hybrid runtime system offering an interactive, secure passthrough <strong>Command Line Console</strong> alongside a high-performance Win32 <strong>Graphical UI Interface</strong>.
</p>

<div class="diagram">
┌────────────────────────────────────────────────────────┐<br>
│ CRON SYSTEM ARCHITECTURE │<br>
└───────────────────────────┬────────────────────────────┘<br>
│<br>
▼<br>
[ Batch 1 ] ──────► [ Batch 2 ] ──────► [ Batch 3 ]<br>
kernel_cmd.bat   kernel_parse.bat   compile_main.bat<br>
(System Probe)   (CDATA Pack)       (MSVC Compiler)<br>
│<br>
▼<br>
[ CRON.exe ]<br>
/ <br>
▼ ▼<br>
[ CLI Terminal ]   [ Win32 GUI Window ]<br>
</div>

<h2>⚙️ Core Architecture & Pipeline Components</h2>

<h3>1. The System Cataloger (<code>kernel_cmd.bat</code>)</h3>
<ul>
  <li><strong>Hardened Path Processing:</strong> Recursively parses <code>%PATH%</code> using a semicolon delimiter to preserve directories with spaces.</li>
  <li><strong>Format Uniformity:</strong> Normalizes text into <code>key=val|usage</code> layout structures.</li>
</ul>

<h3>2. The CDATA Database Parser (<code>kernel_parse.bat</code>)</h3>
<ul>
  <li><strong>Header Elimination:</strong> Strips structural <code>.INI</code> headers to avoid crashes.</li>
  <li><strong>Performance:</strong> Cuts parsing time from 120s to under 2s.</li>
</ul>

<h3>3. The MSVC Compiler Configuration (<code>compile_main.bat</code>)</h3>
<ul>
  <li><strong>Resource Injection:</strong> Embeds <code>favi-con.ico</code> into the binary.</li>
  <li><strong>Optimized Build:</strong> Uses <code>cl.exe /O2 /W4</code> for speed and warnings.</li>
</ul>

<h2>🛠️ Repository File Structures</h2>

<table>
  <tr><th>File Name</th><th>Specification</th><th>Language</th></tr>
  <tr><td><code>kernel_cmd.bat</code></td><td>Enumerates system command sets and paths</td><td>Batch</td></tr>
  <tr><td><code>kernel_parse.bat</code></td><td>Parses configuration variables</td><td>Batch</td></tr>
  <tr><td><code>compile_main.bat</code></td><td>Automated build pipeline</td><td>Batch</td></tr>
  <tr><td><code>resource.rc</code></td><td>Maps system icons</td><td>Resource Script</td></tr>
  <tr><td><code>main.c</code></td><td>Main entry point loop</td><td>C / Win32</td></tr>
  <tr><td><code>avis_window.c</code></td><td>GUI window frame</td><td>C / Win32</td></tr>
  <tr><td><code>cdata_loader.c</code></td><td>Database loader</td><td>C / Win32</td></tr>
  <tr><td><code>avis_window.h</code></td><td>Window manager interface</td><td>Header</td></tr>
  <tr><td><code>cdata_loader.h</code></td><td>Database boundaries</td><td>Header</td></tr>
</table>

<h2>🚀 Building & Running the Project</h2>

<h3>Prerequisites</h3>
<ul>
  <li>Windows 11 (x64)</li>
  <li>Visual Studio 2022 or Build Tools with C++ workload</li>
  <li><code>favi-con.ico</code> in root folder</li>
</ul>

<h3>Compilation Steps</h3>
<pre><code>kernel_cmd.bat
kernel_parse.bat
compile_main.bat
</code></pre>
<p>Generates <strong>CRON.exe</strong> in the root folder.</p>

<h2>⌨️ Execution Options</h2>

<h3>1. CLI Terminal Mode</h3>
<pre><code>CRON.exe</code></pre>
<p>Runs the interactive console editor.</p>

<h3>2. GUI Interface Mode</h3>
<pre><code>CRON.exe --avis-gui</code></pre>
<p>Launches the graphical command builder window.</p>

<div class="diagram">
┌────────────────────────────────────────────────────────────────────────┐<br>
│ CRON Command Builder Window Layout │<br>
├──────────────────────┬─────────────────────────────────────────────────┤<br>
│ [ hUsageBox ]        │ Multi-line help screen monitor │<br>
│ [ hList ]            │ Command list box │<br>
│ [ hEdit ]            │ Syntax customization input │<br>
│ [ hButton ]          │ Send button │<br>
└──────────────────────┴─────────────────────────────────────────────────┘<br>
</div>

<ul>
  <li><strong>Left-Click:</strong> Fills <code>hEdit</code> with usage prototype.</li>
  <li><strong>Right-Click:</strong> Shows usage prototype in <code>hUsageBox</code>.</li>
  <li><strong>Send Button:</strong> Executes the command line.</li>
</ul>

<h2>🛡️ License & Safety Boundaries</h2>
<p>This framework is built strictly around native Windows system environments and administrative tool APIs. Ensure all build scripts maintain correct path mapping access when handling configurations on critical endpoints.</p>

