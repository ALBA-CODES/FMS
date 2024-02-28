<h1>Computational Experiment README</h1>

<h2>Compilation Instructions</h2>

<p>To compile the source code for each problem, use the following commands:</p>

<ul>
  <li>For SSP problem:
    <pre><code>g++ mainSSP.cpp ./src/*.cpp -std=c++20 -Wshadow -Wall -o mainSSP -O3 -Wno-unused-result -lpthread</code></pre>
  </li>
  <li>For IPMTC problem:
    <pre><code>g++ mainIPMTC.cpp ./src/*.cpp -std=c++20 -Wshadow -Wall -o mainIPMTC -O3 -Wno-unused-result -lpthread</code></pre>
  </li>
  <li>For RCPMS problem:
    <pre><code>g++ mainRCPMS.cpp ./src/*.cpp -std=c++20 -Wshadow -Wall -o mainRCPMS -O3 -Wno-unused-result -lpthread</code></pre>
  </li>
</ul>

<h2>Execution Parameters</h2>

<p>To run the application, the following parameters need to be provided:</p>

<h3>For SSP:</h3>
<pre><code>[Instance Name]
--TEMP_INIT Initial Temperature
--TEMP_FIM Final Temperature
--N_REPLICAS Number of Replicas
--MCL Markov Chain Length
--PTL Number of Temperature Changes
--TEMP_DIST Initial Temperature Distribution
--MOV_TYPE Movement Type
--TYPE_UPDATE Temperature Update Type
--TEMP_UPDATE Temperature Update Rate
</code></pre>

<h3>For IPMTC:</h3>
<pre><code>[Instance Name]
--TEMP_INIT Initial Temperature
--TEMP_FIM Final Temperature
--N_REPLICAS Number of Replicas
--MCL Markov Chain Length
--PTL Number of Temperature Changes
--TEMP_DIST Initial Temperature Distribution
--MOV_TYPE Movement Type
--MOV_FORCE Movement Force
--TYPE_UPDATE Temperature Update Type
--TEMP_UPDATE Temperature Update Rate
</code></pre>

<h3>For RCPMS:</h3>
<pre><code>[Instance Name]
--TEMP_INIT Initial Temperature
--TEMP_FIM Final Temperature
--N_REPLICAS Number of Replicas
--MCL Markov Chain Length
--PTL Number of Temperature Changes
--TEMP_DIST Initial Temperature Distribution
--MOV_TYPE Movement Type
--MOV_FORCE Movement Force
--TYPE_UPDATE Temperature Update Type
--TEMP_UPDATE Temperature Update Rate
</code></pre>

<h2>Example Execution</h2>

<h3>For SSP:</h3>
<pre><code>./mainSSP ../instance/Tabela1/T1_F1001.txt --TEMP_INIT 0.2 --TEMP_FIM 1 --N_REPLICAS 16 --MCL 400 --PTL 1000 --TEMP_DIST 1 --MOV_TYPE 1 --TYPE_UPDATE 2 --TEMP_UPDATE 35000
</code></pre>

<h3>For IPMTC:</h3>
<pre><code>./mainIPMTC ../instance/50jobs/instanceLarge50_m\=3_n\=50_l\=30_c\=20_s\=1.PMTC --TEMP_INIT 0.2 --TEMP_FIM 1 --N_REPLICAS 16 --MCL 400 --PTL 1000 --TEMP_DIST 1 --MOV_TYPE 1 --MOV_FORCE 1 --TYPE_UPDATE 2 --TEMP_UPDATE 35000
</code></pre>

<h3>For RCPMS:</h3>
<pre><code>./mainRCPMS ../instance/RCPMSII/RCPMS_Instance_1_m\=3_n\=50_t\=4 --TEMP_INIT 0.2 --TEMP_FIM 1 --N_REPLICAS 16 --MCL 400 --PTL 1000 --TEMP_DIST 1 --MOV_TYPE 1 --MOV_FORCE 1 --TYPE_UPDATE 2 --TEMP_UPDATE 35000
</code></pre>
</html>

