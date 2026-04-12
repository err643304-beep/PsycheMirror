<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>PsycheMirror | Mind to Image EEG Project</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            min-height: 100vh;
            background: radial-gradient(circle at 20% 30%, #0a0f2a, #03050b);
            font-family: 'Segoe UI', 'Poppins', 'Orbitron', 'Inter', system-ui, -apple-system, BlinkMacSystemFont, 'Roboto', sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 2rem;
            position: relative;
            overflow-x: hidden;
        }

        /* animated neural background */
        .neuro-bg {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            z-index: 0;
            pointer-events: none;
            opacity: 0.4;
        }

        .neuro-bg svg {
            width: 100%;
            height: 100%;
        }

        .main-card {
            position: relative;
            z-index: 10;
            max-width: 1100px;
            width: 100%;
            background: rgba(15, 20, 45, 0.55);
            backdrop-filter: blur(12px);
            border-radius: 3.5rem;
            border: 1px solid rgba(255, 255, 255, 0.2);
            box-shadow: 0 30px 50px rgba(0, 0, 0, 0.5), 0 0 0 1px rgba(120, 80, 255, 0.15), 0 0 30px rgba(100, 70, 255, 0.3);
            overflow: hidden;
            transition: all 0.3s ease;
        }

        /* colorful gradient header */
        .psyche-header {
            background: linear-gradient(135deg, #ff5f6d, #ffc371, #2b59ff, #a450ff);
            background-size: 300% 300%;
            animation: gradientFlow 8s ease infinite;
            padding: 2rem 2rem 1.8rem;
            text-align: center;
            border-bottom: 2px solid rgba(255, 255, 255, 0.5);
        }

        @keyframes gradientFlow {
            0% { background-position: 0% 50%; }
            50% { background-position: 100% 50%; }
            100% { background-position: 0% 50%; }
        }

        .psyche-header h1 {
            font-size: 4rem;
            font-weight: 800;
            letter-spacing: 4px;
            background: linear-gradient(135deg, #fff, #ffe6b3, #ffefc0);
            -webkit-background-clip: text;
            background-clip: text;
            color: transparent;
            text-shadow: 0 2px 10px rgba(0,0,0,0.2);
            word-break: keep-all;
        }

        .tagline {
            font-size: 1.3rem;
            font-weight: 500;
            color: #fff4e0;
            text-shadow: 0 0 8px #ff8c00;
            margin-top: 0.5rem;
            letter-spacing: 2px;
        }

        .content {
            padding: 2.5rem;
        }

        .description {
            background: rgba(10, 15, 35, 0.65);
            border-radius: 2rem;
            padding: 1.8rem;
            margin-bottom: 2rem;
            border-left: 6px solid #ff8c6b;
            box-shadow: 0 8px 20px rgba(0,0,0,0.3);
            backdrop-filter: blur(4px);
        }

        .description p {
            color: #f0f3ff;
            font-size: 1.1rem;
            line-height: 1.55;
            font-weight: 400;
            text-shadow: 0 1px 1px black;
        }

        .highlight {
            color: #ffdb8e;
            font-weight: 600;
        }

        .warning-box {
            background: linear-gradient(145deg, rgba(220, 40, 40, 0.2), rgba(160, 20, 30, 0.4));
            border: 2px solid #ff4d4d;
            border-radius: 1.8rem;
            padding: 1.2rem 1.8rem;
            margin-bottom: 2rem;
            display: flex;
            align-items: center;
            gap: 1rem;
            flex-wrap: wrap;
            backdrop-filter: blur(8px);
            box-shadow: 0 0 15px rgba(255, 0, 0, 0.3);
            animation: pulseWarning 1.8s infinite;
        }

        @keyframes pulseWarning {
            0% { border-color: #ff4d4d; box-shadow: 0 0 0 0 rgba(255, 50, 50, 0.5);}
            100% { border-color: #ff9999; box-shadow: 0 0 0 10px rgba(255, 50, 50, 0);}
        }

        .warning-icon {
            font-size: 2.2rem;
            filter: drop-shadow(0 0 4px red);
        }

        .warning-text {
            flex: 1;
            font-weight: 800;
            font-size: 1.3rem;
            color: #ffc5c5;
            text-transform: uppercase;
            letter-spacing: 2px;
            word-break: break-word;
        }

        .warning-sub {
            font-size: 0.9rem;
            color: #ffb0b0;
            font-weight: 500;
        }

        .apps-grid {
            display: flex;
            flex-wrap: wrap;
            gap: 1.2rem;
            margin-top: 1rem;
            justify-content: space-between;
        }

        .app-card {
            background: linear-gradient(135deg, rgba(70, 40, 120, 0.7), rgba(30, 20, 70, 0.8));
            border-radius: 1.5rem;
            padding: 1rem 1.5rem;
            flex: 1;
            min-width: 160px;
            text-align: center;
            border: 1px solid rgba(200, 160, 255, 0.5);
            transition: transform 0.2s, background 0.3s;
            backdrop-filter: blur(5px);
        }

        .app-card:hover {
            transform: translateY(-5px);
            background: linear-gradient(135deg, rgba(110, 70, 180, 0.8), rgba(60, 40, 120, 0.9));
            border-color: #ffb86b;
        }

        .app-icon {
            font-size: 2rem;
            display: block;
            margin-bottom: 0.5rem;
        }

        .app-title {
            font-weight: bold;
            color: #ffdd99;
            font-size: 1.1rem;
        }

        .visualizer-section {
            margin-top: 2.5rem;
            background: rgba(0, 0, 0, 0.45);
            border-radius: 2rem;
            padding: 1.5rem;
            text-align: center;
        }

        .section-title {
            font-size: 1.6rem;
            font-weight: 700;
            background: linear-gradient(90deg, #bc9aff, #ffb86c);
            -webkit-background-clip: text;
            background-clip: text;
            color: transparent;
            margin-bottom: 1rem;
        }

        .mind-to-image-demo {
            display: flex;
            flex-direction: row;
            flex-wrap: wrap;
            justify-content: center;
            gap: 2rem;
            align-items: center;
        }

        .eeg-sim {
            background: #11151fcc;
            border-radius: 2rem;
            padding: 1.2rem;
            width: 240px;
            text-align: center;
            border: 1px solid #4d3c78;
        }

        .wave-animation {
            display: flex;
            justify-content: center;
            align-items: flex-end;
            height: 100px;
            gap: 6px;
            margin: 15px 0;
        }

        .wave-bar {
            width: 12px;
            background: linear-gradient(to top, #3b82f6, #a855f7);
            border-radius: 8px;
            transition: height 0.1s ease;
            animation: brainwave 0.6s infinite alternate ease-in-out;
        }

        @keyframes brainwave {
            0% { height: 20px; opacity: 0.6;}
            100% { height: 70px; opacity: 1;}
        }

        .wave-bar:nth-child(1) { animation-duration: 0.5s; animation-delay: 0s; }
        .wave-bar:nth-child(2) { animation-duration: 0.7s; animation-delay: 0.1s; }
        .wave-bar:nth-child(3) { animation-duration: 0.6s; animation-delay: 0.2s; }
        .wave-bar:nth-child(4) { animation-duration: 0.8s; animation-delay: 0.05s; }
        .wave-bar:nth-child(5) { animation-duration: 0.55s; animation-delay: 0.15s; }
        .wave-bar:nth-child(6) { animation-duration: 0.65s; animation-delay: 0.25s; }
        .wave-bar:nth-child(7) { animation-duration: 0.75s; animation-delay: 0.1s; }
        .wave-bar:nth-child(8) { animation-duration: 0.5s; animation-delay: 0.3s; }

        .generated-image-area {
            background: #010018b3;
            border-radius: 1.8rem;
            padding: 1rem;
            width: 260px;
            border: 2px dashed #bb88ff;
        }

        #mindCanvas {
            background: radial-gradient(circle, #1f1a3a, #0b0822);
            border-radius: 1.2rem;
            width: 100%;
            aspect-ratio: 1 / 1;
            display: block;
            margin: 0 auto;
            box-shadow: 0 0 20px rgba(155, 100, 255, 0.6);
            transition: all 0.2s;
        }

        .btn-reflect {
            background: linear-gradient(95deg, #ff6a4b, #b93bff);
            border: none;
            padding: 12px 20px;
            border-radius: 3rem;
            font-weight: bold;
            color: white;
            font-size: 1rem;
            cursor: pointer;
            margin-top: 1rem;
            transition: 0.2s;
            box-shadow: 0 5px 12px #00000055;
        }

        .btn-reflect:hover {
            transform: scale(1.02);
            filter: brightness(1.1);
            box-shadow: 0 0 15px #ff9066;
        }

        footer {
            font-size: 0.7rem;
            text-align: center;
            padding: 1rem;
            color: #aaaaff;
            border-top: 1px solid rgba(255,255,255,0.15);
            background: #02061760;
        }

        @media (max-width: 700px) {
            .psyche-header h1 { font-size: 2.5rem; }
            .tagline { font-size: 1rem; }
            .warning-text { font-size: 0.9rem; }
            .content { padding: 1.5rem; }
        }
    </style>
</head>
<body>

<div class="neuro-bg">
    <svg viewBox="0 0 1200 800" xmlns="http://www.w3.org/2000/svg">
        <defs>
            <linearGradient id="neuralGrad" x1="0%" y1="0%" x2="100%" y2="100%">
                <stop offset="0%" stop-color="#ff61d6" stop-opacity="0.2"/>
                <stop offset="100%" stop-color="#3a86ff" stop-opacity="0.1"/>
            </linearGradient>
        </defs>
        <path d="M0,400 Q150,200 300,350 T600,300 T900,400 T1200,250" stroke="#ff78c4" fill="none" stroke-width="2" opacity="0.3"/>
        <path d="M0,500 Q200,600 400,450 T700,550 T1100,500 T1200,600" stroke="#6c8eff" fill="none" stroke-width="1.5" opacity="0.25"/>
        <circle cx="200" cy="300" r="3" fill="#ffaa77" opacity="0.6"/>
        <circle cx="500" cy="220" r="2" fill="#ff77aa" opacity="0.7"/>
        <circle cx="850" cy="380" r="3" fill="#aaffdd" opacity="0.5"/>
        <circle cx="1000" cy="280" r="2" fill="#ffb86b" opacity="0.8"/>
    </svg>
</div>

<div class="main-card">
    <div class="psyche-header">
        <h1>🧠 PsycheMirror</h1>
        <div class="tagline">✨ Reflecting the mind's eye · Mind to Image ✨</div>
    </div>

    <div class="content">
        <!-- MAIN DESCRIPTION -->
        <div class="description">
            <p>🧬 <span class="highlight">PsycheMirror</span> is an EEG-based mind-to-image system that translates neural activity into visual representations of thought. By capturing electrical patterns from the brain's visual and associative cortices, <strong class="highlight">PsycheMirror</strong> reconstructs mental imagery — from fleeting memories and imagined scenes to abstract concepts — and renders them as viewable digital images. Acting as a true mirror of the psyche, the system decodes how the mind internally "sees," offering a non-invasive window into subjective experience.</p>
        </div>

        <!-- DANGEROUS WARNING SECTION (custom style) -->
        <div class="warning-box">
            <div class="warning-icon">⚠️🧠💀</div>
            <div class="warning-text">
                ⚡ WARNING THIS TOOL IS DANGEROUS FOR MANKIND ⚡
                <div class="warning-sub">USE IT CAREFULLY. Ethical protocols mandatory. Unauthorized decoding may breach mental privacy.</div>
            </div>
            <div class="warning-icon">🔞🧿</div>
        </div>

        <!-- APPLICATIONS RANGE -->
        <div style="margin-bottom: 0.8rem;">
            <div style="font-size:1.3rem; font-weight:600; color:#cbadff;">📡 APPLICATIONS RANGE</div>
        </div>
        <div class="apps-grid">
            <div class="app-card"><span class="app-icon">🎨✨</span><div class="app-title">Creative Visualization Tools</div></div>
            <div class="app-card"><span class="app-icon">📓🌙</span><div class="app-title">Dream Journaling Aids</div></div>
            <div class="app-card"><span class="app-icon">🫂🔓</span><div class="app-title">Assistive Communication<br>(Locked-in Syndrome / Aphantasia)</div></div>
            <div class="app-card"><span class="app-icon">🔮🧩</span><div class="app-title">Abstract Concept Decoding</div></div>
        </div>

        <!-- INTERACTIVE MIND TO IMAGE DEMO (COLORFUL & PLAYFUL) -->
        <div class="visualizer-section">
            <div class="section-title">🌀 live neuro-visualization | thought → image 🌀</div>
            <div class="mind-to-image-demo">
                <div class="eeg-sim">
                    <div style="font-weight:bold; color:#d9c8ff;">🧠 EEG WAVES (live)</div>
                    <div class="wave-animation" id="waveContainer">
                        <!-- 8 animated bars -->
                        <div class="wave-bar"></div><div class="wave-bar"></div><div class="wave-bar"></div><div class="wave-bar"></div>
                        <div class="wave-bar"></div><div class="wave-bar"></div><div class="wave-bar"></div><div class="wave-bar"></div>
                    </div>
                    <div style="font-size:0.7rem; color:#c7b9ff;">simulated cortical rhythms</div>
                </div>
                <div class="generated-image-area">
                    <div style="font-weight:bold; color:#ffcf9a;">✨ PsycheMirror output ✨</div>
                    <canvas id="mindCanvas" width="200" height="200" style="width:100%; height:auto; max-width:200px; border-radius:20px;"></canvas>
                    <button id="generateMindBtn" class="btn-reflect">🪞 generate mental image 🪞</button>
                    <div style="font-size:0.7rem; margin-top:8px;">decoding neural patterns → visual dreamscape</div>
                </div>
            </div>
            <p style="font-size:0.75rem; margin-top:1rem; color:#b9c7ff;">*dynamic representation: EEG signals translated into kaleidoscopic imagery — abstract mindscapes reflect inner world</p>
        </div>
    </div>
    <footer>
        ⚡ PsycheMirror ·  Ethical AI & Neuroethics first · For research & consensual augmentation only ⚡
    </footer>
</div>

<script>
    (function() {
        // colorful canvas image generation based on "thought" randomness but also deterministic playful patterns
        const canvas = document.getElementById('mindCanvas');
        const ctx = canvas.getContext('2d');
        
        // Set actual canvas resolution for crispness (200x200 already defined, but ensure good quality)
        canvas.width = 200;
        canvas.height = 200;
        
        // Function to draw a colorful abstract "mind-to-image" representation
        function drawMindImage(seed = null) {
            // Use timestamp or random but produce vibrant psychedelic imagery
            let timeVal = Date.now() / 1000;
            if (seed !== undefined && seed !== null) {
                timeVal = seed;
            }
            
            // Clear canvas
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            
            // create dynamic shifting background gradient
            const grad = ctx.createLinearGradient(0, 0, canvas.width, canvas.height);
            const hue1 = (timeVal * 20) % 360;
            const hue2 = (hue1 + 140) % 360;
            grad.addColorStop(0, `hsl(${hue1}, 80%, 55%)`);
            grad.addColorStop(0.5, `hsl(${(hue1 + 70) % 360}, 85%, 45%)`);
            grad.addColorStop(1, `hsl(${hue2}, 80%, 60%)`);
            ctx.fillStyle = grad;
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            
            // Draw fractal-like / neural pattern (colorful circles, waves, mind motifs)
            for (let i = 0; i < 180; i++) {
                const angle = (i * 37 + timeVal * 50) % 360;
                const rad = angle * Math.PI / 180;
                const radius = 20 + Math.sin(angle * 3 + timeVal) * 15;
                const x = canvas.width/2 + Math.cos(rad) * (40 + Math.sin(angle * 2 + timeVal) * 20);
                const y = canvas.height/2 + Math.sin(rad) * (40 + Math.cos(angle * 1.7 + timeVal) * 18);
                const size = 3 + Math.sin(angle * 4 + timeVal) * 2;
                const circleHue = (angle + timeVal * 80) % 360;
                ctx.beginPath();
                ctx.arc(x, y, size, 0, Math.PI*2);
                ctx.fillStyle = `hsla(${circleHue}, 85%, 65%, 0.8)`;
                ctx.fill();
                ctx.shadowBlur = 3;
                ctx.shadowColor = "white";
            }
            
            // draw wavy lines reminiscent of thought patterns
            ctx.beginPath();
            for (let x = 0; x <= canvas.width; x += 12) {
                const yOffset = Math.sin(x * 0.05 + timeVal * 8) * 12 + Math.cos(x * 0.12 + timeVal * 4) * 8;
                const y = canvas.height/2 + yOffset + Math.sin(x * 0.2) * 5;
                if (x === 0) ctx.moveTo(x, y);
                else ctx.lineTo(x, y);
            }
            ctx.strokeStyle = `hsl(${(timeVal * 30) % 360}, 100%, 70%)`;
            ctx.lineWidth = 2.5;
            ctx.stroke();
            
            // additional dreamy dots (neural spikes)
            for (let s = 0; s < 60; s++) {
                const randX = (Math.sin(s * 77 + timeVal) * 0.5 + 0.5) * canvas.width;
                const randY = (Math.cos(s * 43 + timeVal * 1.2) * 0.5 + 0.5) * canvas.height;
                ctx.beginPath();
                ctx.arc(randX, randY, 2 + Math.sin(s + timeVal)*1.2, 0, Math.PI*2);
                ctx.fillStyle = `rgba(255, 200, 100, 0.9)`;
                ctx.fill();
            }
            
            // inner glow: draw a symbolic "mind eye" pupil
            ctx.beginPath();
            ctx.arc(canvas.width/2, canvas.height/2, 18 + Math.sin(timeVal * 5) * 3, 0, Math.PI*2);
            ctx.fillStyle = `rgba(0,0,0,0.3)`;
            ctx.fill();
            ctx.beginPath();
            ctx.arc(canvas.width/2, canvas.height/2, 9 + Math.cos(timeVal * 4) * 2, 0, Math.PI*2);
            ctx.fillStyle = `#fffaec`;
            ctx.fill();
            ctx.beginPath();
            ctx.arc(canvas.width/2, canvas.height/2, 3, 0, Math.PI*2);
            ctx.fillStyle = `#ff55aa`;
            ctx.fill();
            ctx.shadowBlur = 0;
        }
        
        // function to generate random but beautiful shift
        function refreshImageWithNeuralNoise() {
            const dynamicSeed = Date.now() / 230;
            drawMindImage(dynamicSeed);
        }
        
        // auto update every 1.8 seconds to simulate real-time EEG stream -> mind image flow (colorful)
        let intervalAuto;
        
        function startAutoImaging() {
            if(intervalAuto) clearInterval(intervalAuto);
            intervalAuto = setInterval(() => {
                refreshImageWithNeuralNoise();
            }, 2000);
        }
        
        // also make button generate fresh image and add extra animation
        const genBtn = document.getElementById('generateMindBtn');
        if (genBtn) {
            genBtn.addEventListener('click', () => {
                // generate with new entropy & also add "flash" effect
                refreshImageWithNeuralNoise();
                // temporary button feedback
                genBtn.style.transform = 'scale(0.96)';
                setTimeout(() => { if(genBtn) genBtn.style.transform = ''; }, 150);
                // add extra wave effect simulation
                const bars = document.querySelectorAll('.wave-bar');
                bars.forEach((bar, idx) => {
                    const randDur = 0.3 + Math.random() * 0.5;
                    bar.style.animation = 'none';
                    setTimeout(() => {
                        bar.style.animation = `brainwave ${randDur}s infinite alternate ease-in-out`;
                    }, 10);
                });
            });
        }
        
        // simulate dynamic brainwave colors: make the wave bars more colorful by random dynamic heights (already via CSS)
        // add extra vibe: change wave bar backgrounds periodically 
        function updateWaveColors() {
            const bars = document.querySelectorAll('.wave-bar');
            const hue = (Date.now() / 50) % 360;
            bars.forEach((bar, i) => {
                const gradientColor = `linear-gradient(to top, hsl(${hue + i * 15}, 85%, 60%), hsl(${hue + 40 + i * 12}, 90%, 70%))`;
                bar.style.background = gradientColor;
            });
            requestAnimationFrame(updateWaveColors);
        }
        
        // also add a subtle shimmer effect on warning border + dynamic reflection
        let warningBox = document.querySelector('.warning-box');
        if(warningBox) {
            setInterval(() => {
                const intensity = Math.sin(Date.now() / 800) * 0.5 + 0.5;
                const glowColor = `rgba(255, 70, 70, ${0.2 + intensity * 0.3})`;
                warningBox.style.boxShadow = `0 0 12px ${glowColor}, 0 0 5px red`;
            }, 400);
        }
        
        // initial draw
        drawMindImage(Math.PI * 1.618);
        startAutoImaging();
        updateWaveColors();
        
        // additional hover effect on app cards
        const cards = document.querySelectorAll('.app-card');
        cards.forEach(card => {
            card.addEventListener('mouseenter', () => {
                card.style.background = "linear-gradient(135deg, rgba(170, 100, 220, 0.9), rgba(100, 60, 170, 0.95))";
                card.style.transition = "0.2s";
            });
            card.addEventListener('mouseleave', () => {
                card.style.background = "linear-gradient(135deg, rgba(70, 40, 120, 0.7), rgba(30, 20, 70, 0.8))";
            });
        });
        
        // Prevent accidental dangerous misuse message on console (friendly)
        console.log("%c⚠️ PsycheMirror | Mind-to-Image EEG Interface | USE RESPONSIBLY ⚠️", "color: #ff6666; font-size: 14px; font-weight: bold;");
        console.log("%cThis tool simulates neural decoding concepts. Real-world application requires strict ethical oversight.", "color: #ffaa88");
    })();
</script>
</body>
</html>
