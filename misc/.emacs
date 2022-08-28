(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(auto-save-default nil)
 '(auto-save-interval 0)
 '(auto-save-list-file-prefix nil)
 '(auto-save-timeout 0)
 '(auto-show-mode t t)
 '(delete-auto-save-files nil)
 '(delete-old-versions 'other)
 '(imenu-auto-rescan t)
 '(imenu-auto-rescan-maxout 500000)
 '(kept-new-versions 5)
 '(kept-old-versions 5)
 '(make-backup-file-name-function 'ignore)
 '(make-backup-files nil)
 '(mouse-wheel-follow-mouse nil)
 '(mouse-wheel-progressive-speed nil)
 '(mouse-wheel-scroll-amount '(15))
 '(package-selected-packages '(smartparens csharp-mode))
 '(version-control nil))

;; Don't show the splash screen
(setq inhibit-startup-message t)

(setq inhibit-startup-message t) ; Don't show the splash screen
(setq visible-bell t)            ; Flash when the bell rings

; Turn off some unneeded UI elements
(menu-bar-mode -1)  ; Leave this one on if you're a beginner!
(tool-bar-mode -1)
(scroll-bar-mode -1)

(recentf-mode 1)

;; Save what you enter into minibuffer prompts
(setq history-length 25)
(savehist-mode 1)

;; Remember and restore the last cursor location of opened files
(save-place-mode 1)

;; Don't pop up UI dialogs when prompting
(setq use-dialog-box nil)

;; Revert buffers when the underlying file has changed
(global-auto-revert-mode 1)

;; Revert Dired and other buffers
(setq global-auto-revert-non-file-buffers t)

; Stop Emacs from losing undo information by
; setting very high limits for undo buffers
(setq undo-limit 20000000)
(setq undo-strong-limit 40000000)

;packages
(require 'paren)
(require 'cc-mode)
(require 'ido)
(ido-mode t)

; Setup my find-files
(define-key global-map "\ef" 'find-file)
(define-key global-map "\eF" 'find-file-other-window)

(global-set-key (read-kbd-macro "\eb")  'ido-switch-buffer)
(global-set-key (read-kbd-macro "\eB")  'ido-switch-buffer-other-window)

; no screwing with my middle mouse button
(global-unset-key [mouse-2])

; 4-space tabs
(setq-default tab-width 4)
(setq-default indent-tabs-mode nil)

; No hungry backspace
(c-toggle-auto-hungry-state -1)

; Abbrevation expansion
(abbrev-mode 1)

(define-key global-map "\ew" 'other-window)

; Buffers
(define-key global-map "\er" 'revert-buffer)
(define-key global-map "\ek" 'kill-this-buffer)
(define-key global-map "\es" 'save-buffer)
(define-key global-map "\e1" 'delete-window)
(define-key global-map "\e2" 'split-window-horizontally)

; operatoin with text
(define-key global-map "\eo" 'query-replace)

; \377 is alt-backspace
(define-key global-map "\377" 'backward-kill-word)
(define-key global-map [M-delete] 'kill-word)

; Smooth scroll
(setq scroll-step 3)

; Clock
(display-time)

; Startup windowing
(setq next-line-add-newlines nil)
(setq-default truncate-lines t)
(setq truncate-partial-width-windows nil)

(defun casey-never-split-a-window nil)
(setq split-window-preferred-function 'casey-never-split-a-window)

(cua-mode t)

; Additional style stuff
(c-set-offset 'member-init-intro '++)

;;colors
(add-to-list 'default-frame-alist '(font . "Liberation Mono-11.5"))
(set-face-attribute 'default t :font "Liberation Mono-11.5")
(set-face-attribute 'font-lock-builtin-face nil :foreground "#D8A51D")
(set-face-attribute 'font-lock-comment-face nil :foreground "#7D7D7D")
(set-face-attribute 'font-lock-constant-face nil :foreground "#D8A51D")
(set-face-attribute 'font-lock-doc-face nil :foreground "#A6978A")
(set-face-attribute 'font-lock-function-name-face nil :foreground "#7FBF54")
(set-face-attribute 'font-lock-keyword-face nil :foreground "#D95B5B")
(set-face-attribute 'font-lock-string-face nil :foreground "#BFBFBF")
(set-face-attribute 'font-lock-type-face nil :foreground "#56A69C")
(set-face-attribute 'font-lock-variable-name-face nil :foreground "#A6978A")
(set-face-attribute 'region nil :background "#49483E")
(set-foreground-color "#A6978A")
(set-background-color "#1F1F1F")
(set-cursor-color "#7FBF54")
; status bar color
(set-face-foreground 'mode-line "#DBDBDB")
(set-face-background 'mode-line "#A6978A")
(set-face-background 'mode-line-inactive "#7D7D7D")
(set-face-foreground 'mode-line-inactive "#DBDBDB")

;; modeline settings
(setq vc-handled-backends nil) 

;; brackets highlight settings
(setq show-paren-delay 0)
(show-paren-mode 1)
(set-face-background 'show-paren-match "#1F1F1F")
(set-face-foreground 'show-paren-match "#D95B5B")

;;tabulation in modes
(setq c-default-style "bsd" 
      c-basic-offset 4)

(electric-pair-mode 1)

;; Modding c-hook
(add-to-list 'c-mode-common-hook
  (lambda () (setq c-tab-always-indent t)
             (setq c-comment-only-line-offset 0)
             (setq c-electric-pound-behavior nil)))

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
