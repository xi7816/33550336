import { shallowRef, watch, onScopeDispose, type ShallowRef } from 'vue'
import type { UseAudio } from './useAudio'
import type { SwitchSongSnapshot } from '@/domain/types'
import { createTransitionPlayer, TRANSITION_URL, type TransitionPlayer } from './transitionPlayer'

export interface UseSwitchSong {
  snapshot: ShallowRef<SwitchSongSnapshot>
  handleClick: () => void
}

const FALLBACK_TIMEOUT_MS = 30000

export function useSwitchSong(audio: UseAudio): UseSwitchSong {
  const snapshot = shallowRef<SwitchSongSnapshot>({
    state: 'Idle',
    lastResult: 'Success'
  })

  const transitionPlayer: TransitionPlayer = createTransitionPlayer()
  let fallbackTimer: ReturnType<typeof setTimeout> | null = null

  function clearFallbackTimer(): void {
    if (fallbackTimer !== null) {
      clearTimeout(fallbackTimer)
      fallbackTimer = null
    }
  }

  function finishTransition(handoffToNext: boolean): void {
    clearFallbackTimer()
    if (handoffToNext) {
      const s = audio.snapshot.value
      if (s.bgmSwitch === 'On' && s.bgmAvailable) {
        audio.switchToNextTrack()
      }
    }
    snapshot.value = { state: 'Idle', lastResult: snapshot.value.lastResult }
  }

  function handleTransitionEnded(): void {
    snapshot.value = { state: 'Idle', lastResult: 'Success' }
    finishTransition(true)
  }

  function handleTransitionError(): void {
    console.warn('[useSwitchSong] 过渡曲目不可用，降级直接切下一曲')
    snapshot.value = { state: 'Idle', lastResult: 'TransitionUnavailable' }
    finishTransition(true)
  }

  function handleFallbackTimeout(): void {
    console.warn('[useSwitchSong] 过渡播放超时，降级切下一曲')
    snapshot.value = { state: 'Idle', lastResult: 'TransitionUnavailable' }
    finishTransition(true)
  }

  function handleClick(): void {
    if (snapshot.value.state === 'Transitioning') return
    if (audio.snapshot.value.unlockState === 'Locked') return

    snapshot.value = { state: 'Transitioning', lastResult: snapshot.value.lastResult }

    audio.pauseBgmPlayback()

    transitionPlayer.onEnded(handleTransitionEnded)
    transitionPlayer.onError(handleTransitionError)
    transitionPlayer.play(TRANSITION_URL, audio.snapshot.value.volume)

    const duration = transitionPlayer.getDuration()
    if (!isFinite(duration) || duration <= 0) {
      fallbackTimer = setTimeout(handleFallbackTimeout, FALLBACK_TIMEOUT_MS)
    }
  }

  watch(
    () => audio.snapshot.value.volume,
    (newVolume: number) => {
      if (snapshot.value.state === 'Transitioning') {
        transitionPlayer.setVolume(newVolume)
      }
    }
  )

  onScopeDispose(() => {
    clearFallbackTimer()
    transitionPlayer.dispose()
  })

  return {
    snapshot,
    handleClick
  }
}