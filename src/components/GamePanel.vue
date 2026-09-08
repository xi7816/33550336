<script setup lang="ts">
import { useGame } from '@/composables/useGame'
import { useOpening } from '@/composables/useOpening'
import { useAudio } from '@/composables/useAudio'
import { useSwitchSong } from '@/composables/useSwitchSong'
import TurnIndicator from './TurnIndicator.vue'
import ControlBar from './ControlBar.vue'
import GomokuBoard from './GomokuBoard.vue'
import ColorSelectDialog from './ColorSelectDialog.vue'
import GuessFirstDialog from './GuessFirstDialog.vue'
import MusicPanel from './MusicPanel.vue'

const audio = useAudio()
const game = useGame(audio)
const opening = useOpening()
const switchSong = useSwitchSong(audio)

const { snapshot, initGame, placeStone, undo, restart } = game
const {
  snapshot: openingSnapshot,
  confirm,
  selectSize,
  rollNumber,
  guess,
  reset
} = opening
const {
  snapshot: audioSnapshot,
  unlock,
  toggleBgm,
  setVolume,
  toggleSfx
} = audio

opening.onCompleted((size, firstPlayer) => {
  initGame(size, firstPlayer)
})

function handleInit(size: number): void {
  const firstPlayer = openingSnapshot.value.firstPlayer
  if (firstPlayer) {
    initGame(size, firstPlayer)
  }
}

function handleRestart(): void {
  restart()
  reset()
}
</script>

<template>
  <div
    class="game-panel"
    @pointerdown.once="unlock"
  >
    <MusicPanel
      :snapshot="audioSnapshot"
      :switch-song-snapshot="switchSong.snapshot.value"
      @toggle-bgm="toggleBgm"
      @set-volume="setVolume"
      @toggle-sfx="toggleSfx"
      @switch-song-click="switchSong.handleClick"
    />

    <ColorSelectDialog
      :visible="openingSnapshot.phase === 'ColorSelect'"
      :snapshot="openingSnapshot"
      @confirm="confirm"
      @select-size="selectSize"
    />

    <GuessFirstDialog
      :visible="openingSnapshot.phase === 'GuessFirst'"
      :snapshot="openingSnapshot"
      @roll-number="rollNumber"
      @guess="guess"
    />

    <template v-if="openingSnapshot.phase === 'Completed'">
      <TurnIndicator
        :current-player="snapshot.currentPlayer"
        :game-state="snapshot.gameState"
      />
      <GomokuBoard
        :board="snapshot.board"
        :winning-line="snapshot.winningLine"
        :disabled="snapshot.gameState !== 'InProgress'"
        @place="placeStone"
      />
      <ControlBar
        :game-state="snapshot.gameState"
        :history-length="snapshot.history.length"
        :current-size="snapshot.boardSize"
        @init="handleInit"
        @undo="undo"
        @restart="handleRestart"
      />
    </template>
  </div>
</template>

<style scoped>
.game-panel {
  display: flex;
  flex-direction: column;
  align-items: center;
  min-height: 100vh;
  width: 100%;
}
</style>
