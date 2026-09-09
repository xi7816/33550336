<script setup lang="ts">
import type { Player } from '@/domain/types'

defineProps<{
  owner: Player
  active: boolean
  neutral: boolean
}>()
</script>

<template>
  <div
    class="chess-jar"
    :data-owner="owner"
    :class="{ 'is-active': active, 'is-neutral': neutral }"
  >
    <div class="jar-rim" />
    <div class="jar-body">
      <span class="jar-label">{{ owner === 'Black' ? '黑' : '白' }}</span>
    </div>
    <div class="jar-base" />
  </div>
</template>

<style scoped>
.chess-jar {
  --jar-paper: #e8d8b8;
  --jar-wood: #8b7355;
  --jar-wood-dark: #5c4a32;
  --jar-bronze: #6b7a4f;
  --jar-ceramic: #d4c098;
  --jar-ink: #1a1a1a;
  --jar-bone: #f5f5f5;
  --jar-active-glow: 0 0 18px rgba(217, 164, 65, 0.6);
  --jar-transition: 200ms cubic-bezier(0.4, 0, 0.2, 1);

  --jar-candle: #d9a441;
  --jar-bronze-deep: #4a5a3a;
  --jar-wood-light: #a08866;
  --jar-ceramic-warm: #e8d4a8;
  --jar-grain-dark: rgba(60, 45, 30, 0.22);
  --jar-grain-light: rgba(160, 136, 102, 0.12);
  --jar-crackle: rgba(90, 70, 45, 0.10);
  --jar-glow-candle: 0 0 18px rgba(217, 164, 65, 0.6);
  --jar-glow-soft: 0 0 10px rgba(217, 164, 65, 0.3);
  --jar-breathe-duration: 2.4s;
  --jar-gloss-duration: 3.2s;

  display: flex;
  flex-direction: column;
  align-items: center;
  flex: 0 0 auto;
  width: clamp(80px, 12vw, 140px);
  transition: var(--jar-transition);
}

.jar-rim {
  position: relative;
  width: 92%;
  height: clamp(10px, 1.8vh, 16px);
  border-radius: 50%;
  background: linear-gradient(180deg, var(--jar-bronze), var(--jar-bronze-deep));
  border: 1px solid var(--jar-wood-dark);
  box-shadow:
    inset 0 2px 3px rgba(0, 0, 0, 0.35),
    inset 0 -1px 2px rgba(255, 255, 255, 0.10);
  transition: var(--jar-transition);
  overflow: hidden;
}

.jar-rim::after {
  content: '';
  position: absolute;
  inset: 0;
  border-radius: 50%;
  background:
    radial-gradient(circle at 25% 40%, rgba(40, 50, 30, 0.40) 0%, transparent 18%),
    radial-gradient(circle at 70% 60%, rgba(40, 50, 30, 0.35) 0%, transparent 15%),
    radial-gradient(circle at 50% 30%, rgba(60, 70, 45, 0.30) 0%, transparent 20%);
  pointer-events: none;
}

.jar-body {
  position: relative;
  width: 100%;
  height: clamp(92px, 15vh, 152px);
  border-radius: 22px 22px 30px 30px / 26px 26px 34px 34px;
  display: flex;
  align-items: center;
  justify-content: center;
  border: 2px solid var(--jar-wood-dark);
  transition: var(--jar-transition);
  margin-top: -3px;
  overflow: hidden;
}

.chess-jar[data-owner='Black'] .jar-body {
  background:
    repeating-linear-gradient(90deg, var(--jar-grain-dark) 0px, transparent 2px, var(--jar-grain-light) 4px, transparent 7px),
    radial-gradient(ellipse at 50% 20%, rgba(245, 245, 245, 0.15) 0%, transparent 50%),
    linear-gradient(180deg, var(--jar-wood) 0%, var(--jar-wood-dark) 55%, var(--jar-ink) 100%);
  border-color: var(--jar-ink);
}

.chess-jar[data-owner='White'] .jar-body {
  background:
    radial-gradient(ellipse at 50% 20%, rgba(255, 255, 255, 0.55) 0%, transparent 50%),
    linear-gradient(180deg, var(--jar-bone) 0%, var(--jar-ceramic-warm) 50%, var(--jar-wood-light) 100%);
  border-color: var(--jar-wood);
}

.chess-jar[data-owner='White'] .jar-body::before {
  content: '';
  position: absolute;
  inset: 0;
  background:
    repeating-linear-gradient(35deg, var(--jar-crackle) 0px, transparent 3px, var(--jar-crackle) 6px, transparent 11px),
    repeating-linear-gradient(-55deg, var(--jar-crackle) 0px, transparent 4px, var(--jar-crackle) 8px, transparent 14px);
  pointer-events: none;
  z-index: 0;
}

.jar-label {
  position: relative;
  z-index: 1;
  font-size: clamp(22px, 4vw, 34px);
  font-weight: 700;
  font-family: 'KaiTi', 'STKaiti', 'SimSun', serif;
  letter-spacing: 2px;
  text-shadow: 0 1px 2px rgba(0, 0, 0, 0.3);
}

.chess-jar[data-owner='Black'] .jar-label {
  color: var(--jar-bone);
}

.chess-jar[data-owner='White'] .jar-label {
  color: var(--jar-ink);
  text-shadow: 0 1px 2px rgba(0, 0, 0, 0.15);
}

.jar-base {
  width: 80%;
  height: clamp(7px, 1.2vh, 11px);
  border-radius: 50%;
  background: linear-gradient(180deg, var(--jar-wood-dark), var(--jar-ink));
  margin-top: -4px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.3);
  transition: var(--jar-transition);
}

.chess-jar.is-active .jar-body {
  border-width: 3px;
  box-shadow: var(--jar-glow-candle);
  filter: brightness(1.15);
  animation: jar-candle-breathe var(--jar-breathe-duration) ease-in-out infinite;
}

.chess-jar.is-active .jar-rim {
  filter: brightness(1.2);
  box-shadow:
    inset 0 2px 3px rgba(0, 0, 0, 0.35),
    0 0 10px rgba(217, 164, 65, 0.5);
}

.chess-jar.is-active[data-owner='Black'] .jar-body {
  border-color: var(--jar-wood-light);
}

.chess-jar.is-active[data-owner='White'] .jar-body {
  border-color: var(--jar-bronze);
}

.chess-jar:not(.is-active):not(.is-neutral) .jar-body {
  filter: brightness(0.68) saturate(0.6);
  opacity: 0.78;
}

.chess-jar:not(.is-active):not(.is-neutral) .jar-rim,
.chess-jar:not(.is-active):not(.is-neutral) .jar-base {
  filter: brightness(0.7);
  opacity: 0.78;
}

.chess-jar.is-neutral .jar-body {
  filter: brightness(0.58) saturate(0.4);
  opacity: 0.6;
  border-color: var(--jar-wood-dark);
}

.chess-jar.is-neutral .jar-rim,
.chess-jar.is-neutral .jar-base {
  filter: brightness(0.6);
  opacity: 0.6;
}

@keyframes jar-candle-breathe {
  0%, 100% { box-shadow: 0 0 14px rgba(217, 164, 65, 0.5); }
  50% { box-shadow: 0 0 24px rgba(217, 164, 65, 0.78); }
}

@media (max-width: 768px) {
  .chess-jar {
    width: clamp(56px, 18vw, 88px);
  }
  .jar-body {
    height: clamp(70px, 12vh, 110px);
  }
  .jar-label {
    font-size: clamp(18px, 5vw, 26px);
  }
  .chess-jar[data-owner='White'] .jar-body::before {
    display: none;
  }
  .chess-jar.is-active .jar-body {
    animation: none;
  }
}

@media (prefers-reduced-motion: reduce) {
  .chess-jar.is-active .jar-body {
    animation: none;
  }
  .chess-jar,
  .jar-rim,
  .jar-body,
  .jar-base {
    transition: none;
  }
}
</style>
