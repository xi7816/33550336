<script setup lang="ts">
import { useGame } from '@/composables/useGame'
import TurnIndicator from './TurnIndicator.vue'
import ControlBar from './ControlBar.vue'
import GomokuBoard from './GomokuBoard.vue'

const { snapshot, initGame, placeStone, undo, restart } = useGame()
</script>

<template>
  <div class="game-panel">
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
      @init="initGame"
      @undo="undo"
      @restart="restart"
    />
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
