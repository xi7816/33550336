import { computed, toValue, type ComputedRef } from 'vue'
import type { Player, GameState } from '@/domain/types'

export interface DerivedJarState {
  blackActive: boolean
  whiteActive: boolean
  neutral: boolean
}

export function deriveJarState(currentPlayer: Player, gameState: GameState): DerivedJarState {
  switch (gameState) {
    case 'InProgress':
      return currentPlayer === 'Black'
        ? { blackActive: true, whiteActive: false, neutral: false }
        : { blackActive: false, whiteActive: true, neutral: false }
    case 'BlackWin':
      return { blackActive: true, whiteActive: false, neutral: false }
    case 'WhiteWin':
      return { blackActive: false, whiteActive: true, neutral: false }
    case 'Draw':
      return { blackActive: false, whiteActive: false, neutral: true }
    case 'Uninitialized':
      return { blackActive: false, whiteActive: false, neutral: false }
    default:
      return { blackActive: false, whiteActive: false, neutral: false }
  }
}

export interface JarIndicatorState {
  blackActive: ComputedRef<boolean>
  whiteActive: ComputedRef<boolean>
  neutral: ComputedRef<boolean>
}

type Source<T> = ComputedRef<T> | (() => T)

export function useJarIndicator(
  currentPlayer: Source<Player>,
  gameState: Source<GameState>
): JarIndicatorState {
  const state = computed(() => deriveJarState(toValue(currentPlayer), toValue(gameState)))
  const blackActive = computed(() => state.value.blackActive)
  const whiteActive = computed(() => state.value.whiteActive)
  const neutral = computed(() => state.value.neutral)
  return { blackActive, whiteActive, neutral }
}