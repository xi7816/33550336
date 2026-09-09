<script setup lang="ts">
import type { Player, GameState } from '@/domain/types'
import ChessJar from './ChessJar.vue'
import { useJarIndicator } from '@/composables/useJarIndicator'

const props = defineProps<{
  currentPlayer: Player
  gameState: GameState
  visible: boolean
}>()

const { blackActive, whiteActive, neutral } = useJarIndicator(
  () => props.currentPlayer,
  () => props.gameState
)
</script>

<template>
  <div v-if="visible" class="jar-indicator">
    <ChessJar owner="Black" :active="blackActive" :neutral="neutral" />
    <div class="jar-stage-center">
      <slot />
    </div>
    <ChessJar owner="White" :active="whiteActive" :neutral="neutral" />
  </div>
</template>

<style scoped>
.jar-indicator {
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  gap: clamp(10px, 2vw, 20px);
  width: 100%;
}

.jar-stage-center {
  flex: 1 1 auto;
  display: flex;
  justify-content: center;
  align-items: center;
}
</style>