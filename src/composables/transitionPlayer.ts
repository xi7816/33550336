export const TRANSITION_URL = '/transition/gg.mp4'

export interface TransitionPlayer {
  play: (url: string, volume: number) => void
  onEnded: (handler: () => void) => void
  onError: (handler: () => void) => void
  stop: () => void
  dispose: () => void
  getDuration: () => number
  setVolume: (volume: number) => void
}

export function createTransitionPlayer(): TransitionPlayer {
  const audio = new Audio()

  let endedHandler: (() => void) | null = null
  let errorHandler: (() => void) | null = null

  const internalEnded = (): void => {
    if (endedHandler) {
      const h = endedHandler
      endedHandler = null
      audio.removeEventListener('ended', internalEnded)
      h()
    }
  }

  const internalError = (): void => {
    if (errorHandler) {
      const h = errorHandler
      errorHandler = null
      audio.removeEventListener('error', internalError)
      h()
    }
  }

  function play(url: string, volume: number): void {
    audio.src = url
    audio.volume = Math.max(0, Math.min(1, volume / 100))
    audio.play().catch(() => {})
  }

  function onEnded(handler: () => void): void {
    endedHandler = handler
    audio.addEventListener('ended', internalEnded)
  }

  function onError(handler: () => void): void {
    errorHandler = handler
    audio.addEventListener('error', internalError)
  }

  function stop(): void {
    audio.pause()
    audio.src = ''
    audio.removeEventListener('ended', internalEnded)
    audio.removeEventListener('error', internalError)
    endedHandler = null
    errorHandler = null
  }

  function dispose(): void {
    stop()
  }

  function getDuration(): number {
    return audio.duration
  }

  function setVolume(volume: number): void {
    audio.volume = Math.max(0, Math.min(1, volume / 100))
  }

  return {
    play,
    onEnded,
    onError,
    stop,
    dispose,
    getDuration,
    setVolume
  }
}