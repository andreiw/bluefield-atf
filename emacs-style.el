;;; Set emacs C style to be appropriate for editing atf sources

(setq tab-width 8)
(setq indent-tabs-mode t)
(if (and (boundp 'c-buffer-is-cc-mode) c-buffer-is-cc-mode)
    (c-set-style "linux"))
