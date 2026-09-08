import { shallowRef, ref, watch, onScopeDispose, type ShallowRef, type Ref } from 'vue'
import type { UseAudio } from './useAudio'
import type { SwitchSongSnapshot } from '@/domain/types'
import { createTransitionPlayer, TRANSITION_URL, type TransitionPlayer } from './transitionPlayer'

export interface UseSwitchSong {
  snapshot: ShallowRef<SwitchSongSnapshot>
  overlayVisible: Ref<boolean>
  skipButtonVisible: Ref<boolean>
  handleClick: () => void
  handleSkip: () => void
  handleContainerReady: (el: HTMLElement) => void
}

const FALLBACK_TIMEOUT_MS = 30000

export function useSwitchSong(audio: UseAudio): UseSwitchSong {
  const snapshot = shallowRef<SwitchSongSnapshot>({
    state: 'Idle',
    lastResult: 'Success'
  })
  const overlayVisible = ref<boolean>(false)
  const skipButtonVisible = ref<boolean>(false)

  const transitionPlayer: TransitionPlayer = createTransitionPlayer()
  let fallbackTimer: ReturnType<typeof setTimeout> | null = null
  let currentContainer: HTMLElement | null = null
  let finishing = false

  function clearFallbackTimer(): void {
    if (fallbackTimer !== null) {
      clearTimeout(fallbackTimer)
      fallbackTimer = null
    }
  }

  function finishTransition(handoffToNext: boolean): void {
    if (finishing) return
    finishing = true
    clearFallbackTimer()
    overlayVisible.value = false
    skipButtonVisible.value = false
    transitionPlayer.detach()
    currentContainer = null
    if (handoffToNext) {
      const s = audio.snapshot.value
      if (s.bgmSwitch === 'On' && s.bgmAvailable) {
        audio.switchToNextTrack()
      }
    }
    snapshot.value = { state: 'Idle', lastResult: snapshot.value.lastResult }
    finishing = false
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

  function handleContainerReady(el: HTMLElement): void {
    currentContainer = el
    if (snapshot.value.state === 'Transitioning') {
      transitionPlayer.attach(el)
    }
  }

  function handleClick(): void {
    if (snapshot.value.state === 'Transitioning') return
    if (audio.snapshot.value.unlockState === 'Locked') return

    snapshot.value = { state: 'Transitioning', lastResult: snapshot.value.lastResult }
    overlayVisible.value = true
    skipButtonVisible.value = true

    audio.pauseBgmPlayback()

    transitionPlayer.onEnded(handleTransitionEnded)
    transitionPlayer.onError(handleTransitionError)

    if (currentContainer) {
      transitionPlayer.attach(currentContainer)
    }
    transitionPlayer.play(TRANSITION_URL, audio.snapshot.value.volume)

    const duration = transitionPlayer.getDuration()
    if (!isFinite(duration) || duration <= 0) {
      fallbackTimer = setTimeout(handleFallbackTimeout, FALLBACK_TIMEOUT_MS)
    }
  }

  function handleSkip(): void {
    if (snapshot.value.state !== 'Transitioning') return
    if (finishing) return
    skipButtonVisible.value = false
    transitionPlayer.stop()
    finishTransition(true)
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
    overlayVisible.value = false
    transitionPlayer.dispose()
    currentContainer = null
  })

  return {
    snapshot,
    overlayVisible,
    skipButtonVisible,
    handleClick,
    handleSkip,
    handleContainerReady
  }
}
