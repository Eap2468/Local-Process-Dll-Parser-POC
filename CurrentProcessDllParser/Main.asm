.code

GetDllListEntry proc
	xor rax, rax			;Clear rax for operations
	mov rax, QWORD ptr gs:[60h]		;Stores the PEB in rax
	mov rax, [rax + 18h]	;Stores the LDR struct in rax
	mov rax, [rax + 20h]	;Stores the InMemoryOrderModuleList struct in rax
	ret
GetDllListEntry endp
end