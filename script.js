/* =========================================
   1. CONFIGURATION & THEME
   ========================================= */
const THEME = {
    primary: '#6366f1',    // Indigo 500
    primaryDark: '#4f46e5', // Indigo 600
    secondary: '#cbd5e1',  // Slate 300
    bgNode: '#ffffff',     // White
    textMain: '#1e293b',   // Slate 800
    highlight: '#ef4444',  // Red (Active/Processing)
    visited: '#3b82f6',    // Blue (Visited)
    success: '#10b981',    // Emerald (Completed)
    edgeDefault: '#94a3b8' // Slate 400
};

// Global WebAssembly Module Hook
var Module = {
    onRuntimeInitialized: function() {
        log("System: WebAssembly Core Loaded Successfully.", "system");
        initWrappers();
    },
    print: function(text) {
        log("Backend: " + text);
    }
};

/* =========================================
   2. UTILITIES
   ========================================= */
function log(msg, type = 'normal') {
    const box = document.getElementById('log-box');
    const time = new Date().toLocaleTimeString('en-US', { hour12: false });
    
    const entry = document.createElement('div');
    entry.className = `log-entry ${type}`;
    
    // Add icon based on message type
    let icon = type === 'system' ? '<i class="fa-solid fa-server"></i>' : '<i class="fa-solid fa-terminal"></i>';
    
    entry.innerHTML = `<span style="color:#64748b; font-size:0.75rem;">[${time}]</span> ${icon} ${msg}`;
    
    box.appendChild(entry);
    box.scrollTop = box.scrollHeight; // Auto-scroll to bottom
}

function switchTab(id) {
    // 1. Hide all sections
    document.querySelectorAll('.visualizer-section').forEach(el => el.classList.add('hidden'));
    
    // 2. Show target section
    document.getElementById(id).classList.remove('hidden');
    
    // 3. Update Sidebar Navigation
    document.querySelectorAll('.nav-item').forEach(el => el.classList.remove('active'));
    // Find the clicked nav item (simple logic based on onclick handler matching)
    const navItems = document.querySelectorAll('.nav-item');
    if(id === 'avl') navItems[0].classList.add('active');
    if(id === 'heap') navItems[1].classList.add('active');
    if(id === 'hash') navItems[2].classList.add('active');
    if(id === 'graph') navItems[3].classList.add('active');

    log(`UI: Switched to ${id.toUpperCase()} view.`);
}

/* =========================================
   3. WASM WRAPPERS (C++ LINKAGE)
   ========================================= */
let avl, heap, hash, graph;
let _avlIns, _avlDel, _getAvl;
let _heapIns, _heapExt, _getHeap;
let _hashIns, _getHash;
let _graphV, _graphE, _getGraph;

function initWrappers() {
    try {
        // AVL Bindings
        avl = Module.cwrap('createAVL', 'number', [])();
        _avlIns = Module.cwrap('avlInsert', null, ['number','number']);
        _avlDel = Module.cwrap('avlDelete', null, ['number','number']);
        _getAvl = Module.cwrap('getAVLData', 'string', ['number']);

        // Heap Bindings
        heap = Module.cwrap('createHeap', 'number', ['number'])(100);
        _heapIns = Module.cwrap('heapInsert', null, ['number','number']);
        _heapExt = Module.cwrap('heapExtract', 'number', ['number']);
        _getHeap = Module.cwrap('getHeapData', 'string', ['number']);

        // Hash Bindings
        hash = Module.cwrap('createSC', 'number', ['number'])(11);
        _hashIns = Module.cwrap('scInsert', null, ['number','number','string']);
        _getHash = Module.cwrap('getHashData', 'string', ['number']);

        // Graph Bindings
        graph = Module.cwrap('createGraph', 'number', [])();
        _graphV = Module.cwrap('addVertex', null, ['number','number','string']);
        _graphE = Module.cwrap('addEdge', null, ['number','number','number','number']);
        _getGraph = Module.cwrap('getGraphData', 'string', ['number']);

        log("System: All C++ functions bound to JavaScript.");
    } catch (e) {
        log("Error: Failed to bind WebAssembly functions. " + e, "system");
    }
}

/* =========================================
   4. AVL TREE LOGIC
   ========================================= */
function avlOp(op) {
    const input = document.getElementById('avlInput');
    const val = parseInt(input.value);
    
    if (isNaN(val)) {
        log("Error: Please enter a valid integer.");
        return;
    }

    if (op === 'insert') _avlIns(avl, val);
    else _avlDel(avl, val);

    const data = JSON.parse(_getAvl(avl));
    log(`AVL: ${op === 'insert' ? 'Inserted' : 'Deleted'} node ${val}`);
    
    drawTree(data, 'avlCanvas');
    input.value = ''; // Clear input
    input.focus();
}

function drawTree(node, cvsId, x = 550, y = 60, offset = 220) {
    const canvas = document.getElementById(cvsId);
    const ctx = canvas.getContext('2d');
    
    // Clear canvas only on root call (x position check is a simple hack)
    if (x === 550 && y === 60) ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    if (!node) return;

    // Draw Connections First (so lines are behind circles)
    ctx.lineWidth = 3;
    if (node.l) {
        ctx.beginPath();
        ctx.moveTo(x, y + 28);
        ctx.lineTo(x - offset, y + 80);
        ctx.strokeStyle = THEME.secondary;
        ctx.stroke();
        drawTree(node.l, cvsId, x - offset, y + 80, offset * 0.55);
    }
    if (node.r) {
        ctx.beginPath();
        ctx.moveTo(x, y + 28);
        ctx.lineTo(x + offset, y + 80);
        ctx.strokeStyle = THEME.secondary;
        ctx.stroke();
        drawTree(node.r, cvsId, x + offset, y + 80, offset * 0.55);
    }

    // Draw Node Circle
    ctx.beginPath();
    ctx.arc(x, y, 26, 0, 2 * Math.PI);
    ctx.fillStyle = THEME.bgNode;
    ctx.fill();
    
    // Border Color based on Balance Factor
    ctx.strokeStyle = node.h === 0 ? THEME.success : (Math.abs(node.h) > 1 ? THEME.highlight : THEME.primary);
    ctx.stroke();

    // Text
    ctx.fillStyle = THEME.textMain;
    ctx.font = "bold 16px Poppins";
    ctx.textAlign = "center";
    ctx.fillText(node.k, x, y + 6);
    
    // Metadata (Balance Factor)
    ctx.fillStyle = "#64748b";
    ctx.font = "11px Poppins";
    ctx.fillText(`BF:${node.h}`, x, y + 40);
}

/* =========================================
   5. HEAP LOGIC
   ========================================= */
function heapOp(op) {
    const input = document.getElementById('heapInput');
    
    if (op === 'insert') {
        const val = parseInt(input.value);
        if (!isNaN(val)) {
            _heapIns(heap, val);
            log(`Heap: Inserted ${val}`);
            input.value = '';
        }
    } else {
        const val = _heapExt(heap);
        log(`Heap: Extracted Min Value (${val})`);
    }

    const arr = JSON.parse(_getHeap(heap));
    drawHeap(arr);
}

function drawHeap(arr) {
    const ctx = document.getElementById('heapCanvas').getContext('2d');
    ctx.clearRect(0, 0, 1100, 600);
    
    if (arr.length === 0) return;

    const drawNode = (i, x, y, offset) => {
        if (i >= arr.length) return;

        // Draw Links
        const left = 2 * i + 1;
        const right = 2 * i + 2;

        ctx.strokeStyle = THEME.secondary;
        ctx.lineWidth = 3;

        if (left < arr.length) {
            ctx.beginPath(); ctx.moveTo(x, y + 25); ctx.lineTo(x - offset, y + 80); ctx.stroke();
            drawNode(left, x - offset, y + 80, offset / 2);
        }
        if (right < arr.length) {
            ctx.beginPath(); ctx.moveTo(x, y + 25); ctx.lineTo(x + offset, y + 80); ctx.stroke();
            drawNode(right, x + offset, y + 80, offset / 2);
        }

        // Draw Node
        ctx.beginPath();
        ctx.arc(x, y, 25, 0, 2 * Math.PI);
        ctx.fillStyle = THEME.bgNode;
        ctx.fill();
        ctx.strokeStyle = THEME.primary;
        ctx.stroke();

        ctx.fillStyle = THEME.textMain;
        ctx.font = "bold 16px Poppins";
        ctx.textAlign = "center";
        ctx.fillText(arr[i], x, y + 6);
    };

    drawNode(0, 550, 60, 240);
}

/* =========================================
   6. HASH TABLE LOGIC
   ========================================= */
function hashOp() {
    const keyIn = document.getElementById('hashKey');
    const valIn = document.getElementById('hashVal');
    const k = parseInt(keyIn.value);
    const v = valIn.value;

    if (!isNaN(k)) {
        _hashIns(hash, k, v);
        log(`Hash: Inserted key ${k} with value "${v}"`);
        const data = JSON.parse(_getHash(hash));
        drawHash(data);
        keyIn.value = '';
        valIn.value = '';
    }
}

function drawHash(table) {
    const ctx = document.getElementById('hashCanvas').getContext('2d');
    ctx.clearRect(0, 0, 1100, 600);
    ctx.font = "14px Poppins";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";

    table.forEach((bucket, i) => {
        let y = 60 + i * 50;

        // Draw Index Box
        ctx.fillStyle = '#f1f5f9';
        ctx.fillRect(50, y, 50, 40);
        ctx.strokeStyle = '#cbd5e1';
        ctx.lineWidth = 2;
        ctx.strokeRect(50, y, 50, 40);
        
        ctx.fillStyle = '#64748b';
        ctx.fillText(i, 75, y + 20);

        // Draw Chain
        let xStart = 140;
        bucket.forEach((val, j) => {
            let x = xStart + j * 130;

            // Draw Arrow
            ctx.beginPath();
            ctx.moveTo(x - 30, y + 20);
            ctx.lineTo(x, y + 20);
            ctx.strokeStyle = THEME.primary;
            ctx.lineWidth = 2;
            ctx.stroke();

            // Draw Data Box
            ctx.fillStyle = '#fff';
            ctx.shadowColor = "rgba(0,0,0,0.1)";
            ctx.shadowBlur = 6;
            ctx.fillRect(x, y, 100, 40);
            ctx.shadowBlur = 0; // Reset shadow
            
            ctx.strokeStyle = THEME.primaryDark;
            ctx.strokeRect(x, y, 100, 40);

            ctx.fillStyle = THEME.textMain;
            ctx.fillText(val, x + 50, y + 20);
        });
    });
}

/* =========================================
   7. GRAPH LOGIC
   ========================================= */
function graphAddV() {
    const id = parseInt(document.getElementById('vId').value);
    const name = document.getElementById('vName').value;
    if (!isNaN(id)) {
        _graphV(graph, id, name);
        renderGraph();
    }
}

function graphAddE() {
    const u = parseInt(document.getElementById('eU').value);
    const v = parseInt(document.getElementById('eV').value);
    const w = parseInt(document.getElementById('eW').value);
    if (!isNaN(u) && !isNaN(v) && !isNaN(w)) {
        _graphE(graph, u, v, w);
        renderGraph();
    }
}

function graphAlgo(algo) {
    const start = parseInt(document.getElementById('startId').value);
    if (isNaN(start)) {
        log("Error: Please provide a valid Start Node ID.");
        return;
    }
    
    // Check if node exists
    const data = JSON.parse(_getGraph(graph));
    if (!data.nodes.includes(start)) {
        log(`Error: Node ${start} does not exist in the graph.`);
        return;
    }

    log(`Graph: Running ${algo.toUpperCase()} algorithm...`);
    
    if (algo === 'bfs') animateBFS(start);
    else if (algo === 'dfs') animateDFS(start);
    else if (algo === 'prim') animatePrim(start);
    else if (algo === 'dijkstra') animateDijkstra(start);
}

// Graph Utilities
function buildAdj(data) {
    let adj = {};
    data.nodes.forEach(id => adj[id] = []);
    data.edges.forEach(e => {
        adj[e.u].push({to: e.v, w: e.w});
        adj[e.v].push({to: e.u, w: e.w});
    });
    return adj;
}

// --- Animation Functions (Standard Logic) ---

function animateBFS(start) {
    let data = JSON.parse(_getGraph(graph));
    let adj = buildAdj(data);
    let visited = new Set([start]);
    let queue = [start];
    
    function step() {
        if (queue.length === 0) {
            renderGraph({visited});
            log("BFS Completed.", "success");
            return;
        }
        let curr = queue.shift();
        
        // --- LOG RESTORED ---
        log(`BFS: Visiting node ${curr}`); 
        
        renderGraph({highlight: curr, visited, queue: [...queue]});
        
        for (let neigh of adj[curr]) {
            if (!visited.has(neigh.to)) {
                visited.add(neigh.to);
                queue.push(neigh.to);
            }
        }
        setTimeout(step, 1000);
    }
    step();
}

function animateDFS(start) {
    let data = JSON.parse(_getGraph(graph));
    let adj = buildAdj(data);
    let visited = new Set([start]);
    let stack = [start];
    
    function step() {
        if (stack.length === 0) {
            renderGraph({visited});
            log("DFS Completed.", "success");
            return;
        }
        let curr = stack.pop();
        
        // --- LOG RESTORED ---
        log(`DFS: Visiting node ${curr}`);
        
        renderGraph({highlight: curr, visited, stack: [...stack]});
        
        for (let neigh of adj[curr].reverse()) {
            if (!visited.has(neigh.to)) {
                visited.add(neigh.to);
                stack.push(neigh.to);
            }
        }
        setTimeout(step, 1000);
    }
    step();
}

function animatePrim(start) {
    let data = JSON.parse(_getGraph(graph));
    let adj = buildAdj(data);
    let nodes = data.nodes;
    let dist = {};
    let parent = {};
    let visited = new Set();
    
    nodes.forEach(n => dist[n] = Infinity);
    dist[start] = 0;
    
    let treeEdges = [];

    function step() {
        let minD = Infinity, u = null;
        for(let n of nodes) {
            if (!visited.has(n) && dist[n] < minD) {
                minD = dist[n];
                u = n;
            }
        }

        if (u === null) {
            renderGraph({treeEdges, visited, dist});
            log("Prim's MST Completed.", "success");
            return;
        }

        visited.add(u);
        
        // --- LOG RESTORED ---
        log(`Prim: Added ${u} to MST (Distance: ${dist[u]})`);
        
        if (parent[u] !== undefined) treeEdges.push({u: parent[u], v: u});
        
        renderGraph({highlight: u, visited, treeEdges, dist});

        for (let neigh of adj[u]) {
            let v = neigh.to, w = neigh.w;
            if (!visited.has(v) && w < dist[v]) {
                dist[v] = w;
                parent[v] = u;
                // --- LOG RESTORED ---
                log(`Prim: Updated dist[${v}] to ${w}`);
            }
        }
        setTimeout(step, 1200);
    }
    step();
}

function animateDijkstra(start) {
    let data = JSON.parse(_getGraph(graph));
    let adj = buildAdj(data);
    let nodes = data.nodes;
    let dist = {};
    let parent = {};
    let visited = new Set();
    
    nodes.forEach(n => dist[n] = Infinity);
    dist[start] = 0;
    let shortestPathEdges = [];

    function step() {
        let minD = Infinity, u = null;
        for(let n of nodes) {
            if (!visited.has(n) && dist[n] < minD) {
                minD = dist[n];
                u = n;
            }
        }

        if (u === null) {
            renderGraph({visited, dist, treeEdges: shortestPathEdges});
            log("Dijkstra Completed.", "success");
            return;
        }

        visited.add(u);
        if (parent[u] !== undefined) shortestPathEdges.push({u: parent[u], v: u});
        
        // --- LOG RESTORED ---
        log(`Dijkstra: Relaxing node ${u} (Current Dist: ${dist[u]})`);

        renderGraph({highlight: u, visited, dist, treeEdges: shortestPathEdges});

        for (let neigh of adj[u]) {
            let v = neigh.to, w = neigh.w;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                // --- LOG RESTORED ---
                log(`Dijkstra: Updated dist[${v}] to ${dist[v]}`);
            }
        }
        setTimeout(step, 1200);
    }
    step();
}

function renderGraph(options = {}) {
    const canvas = document.getElementById('graphCanvas');
    const ctx = canvas.getContext('2d');
    const width = canvas.width, height = canvas.height;
    
    ctx.clearRect(0, 0, width, height);

    let raw = _getGraph(graph);
    let data = JSON.parse(raw);
    
    // Circular Layout Calculation
    const cx = width / 2, cy = height / 2, r = 200;
    let coords = {};
    data.nodes.forEach((id, i) => {
        let ang = (i / data.nodes.length) * Math.PI * 2;
        coords[id] = { 
            x: cx + Math.cos(ang) * r, 
            y: cy + Math.sin(ang) * r 
        };
    });

    // Draw Edges
    data.edges.forEach(e => {
        let p1 = coords[e.u], p2 = coords[e.v];
        if (p1 && p2) {
            ctx.beginPath();
            ctx.moveTo(p1.x, p1.y);
            ctx.lineTo(p2.x, p2.y);
            
            // Highlight MST/Path edges
            let isHighlight = options.treeEdges && options.treeEdges.some(te => 
                (te.u === e.u && te.v === e.v) || (te.u === e.v && te.v === e.u)
            );
            
            ctx.strokeStyle = isHighlight ? THEME.success : THEME.edgeDefault;
            ctx.lineWidth = isHighlight ? 4 : 2;
            ctx.stroke();

            // Edge Weight
            ctx.fillStyle = "#ef4444";
            ctx.font = "bold 14px Poppins";
            // Draw weight in the middle of line, slightly offset
            ctx.fillText(e.w, (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 - 5);
        }
    });

    // Draw Nodes
    data.nodes.forEach(id => {
        let p = coords[id];
        ctx.beginPath();
        ctx.arc(p.x, p.y, 28, 0, Math.PI * 2);
        
        // Color Logic
        if (options.highlight === id) ctx.fillStyle = THEME.highlight;
        else if (options.visited && options.visited.has(id)) ctx.fillStyle = THEME.visited;
        else ctx.fillStyle = THEME.textMain;
        
        ctx.fill();
        ctx.strokeStyle = '#fff';
        ctx.lineWidth = 3;
        ctx.stroke();

        ctx.fillStyle = '#fff';
        ctx.font = "bold 16px Poppins";
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.fillText(id, p.x, p.y);

        // Distance Labels (Dijkstra/Prim)
        if (options.dist && options.dist[id] !== undefined) {
            let dtext = options.dist[id] === Infinity ? '∞' : options.dist[id];
            ctx.fillStyle = THEME.success;
            ctx.font = "bold 14px Poppins";
            ctx.fillText(dtext, p.x, p.y + 45);
        }
    });

    // Draw Queue/Stack Visualization at bottom
    if (options.queue || options.stack) {
        const arr = options.queue || options.stack;
        const label = options.queue ? "Queue: " : "Stack: ";
        
        ctx.font = "16px Poppins";
        ctx.fillStyle = THEME.textMain;
        ctx.textAlign = "left";
        ctx.fillText(label + JSON.stringify(arr), 50, height - 30);
    }
}
