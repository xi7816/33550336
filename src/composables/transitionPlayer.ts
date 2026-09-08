export const TRANSITION_URL = '/transition/gg.mp4'

export interface TransitionPlayer {
  play: (url: string, volume: number) => void
  onEnded: (handler: () => void) => void
  onError: (handler: () => void) => void
  stop: () => void
  dispose: () => void
  getDuration: () => number
  setVolume: (volume: number) => void
  attach: (container: HTMLElement) => void
  detach: () => void
}

export function createTransitionPlayer(): TransitionPlayer {
  const video = document.createElement('video')
  video.controls = false
  video.loop = false
  video.style.width = '100%'
  video.style.height = '100%'
  video.style.objectFit = 'contain'

  let endedHandler: (() => void) | null = null
  let errorHandler: (() => void) | null = null

  const internalEnded = (): void => {
    if (endedHandler) {
      const h = endedHandler
      endedHandler = null
      video.removeEventListener('ended', internalEnded)
      h()
    }
  }

  const internalError = (): void => {
    if (errorHandler) {
      const h = errorHandler
      errorHandler = null
      video.removeEventListener('error', internalError)
      h()
    }
  }

  function play(url: string, volume: number): void {
    video.src = url
    video.volume = Math.max(0, Math.min(1, volume / 100))
    video.play().catch(() => {})
  }

  function onEnded(handler: () => void): void {
    endedHandler = handler
    video.addEventListener('ended', internalEnded)
  }

  function onError(handler: () => void): void {
    errorHandler = handler
    video.addEventListener('error', internalError)
  }

  function attach(container: HTMLElement): void {
    detach()
    container.appendChild(video)
  }

  function detach(): void {
    if (video.parentNode) {
      video.parentNode.removeChild(video)
    }
  }

  function stop(): void {
    video.pause()
    video.src = ''
    video.removeEventListener('ended', internalEnded)
    video.removeEventListener('error', internalError)
    endedHandler = null
    errorHandler = null
    detach()
  }

  function dispose(): void {
    stop()
  }

  function getDuration(): number {
    return video.duration
  }

  function setVolume(volume: number): void {
    video.volume = Math.max(0, Math.min(1, volume / 100))
  }

  return {
    play,
    onEnded,
    onError,
    stop,
    dispose,
    getDuration,
    setVolume,
    attach,
    detach
  }
}
