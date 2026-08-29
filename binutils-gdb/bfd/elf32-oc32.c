/* OC32-specific support for 32-bit ELF.
   Copyright (C) 2023-2024 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/oc32.h"
#include "opcode/oc32.h"

/* OC32 relocation howto table.  */

static reloc_howto_type oc32_elf_howto_table[] =
	{
		/* No relocation.  */
		HOWTO(R_OC32_NONE,			  /* type */
			  0,					  /* rightshift */
			  0,					  /* size */
			  0,					  /* bitsize */
			  false,				  /* pc_relative */
			  0,					  /* bitpos */
			  complain_overflow_dont, /* complain_on_overflow */
			  bfd_elf_generic_reloc,  /* special_function */
			  "R_OC32_NONE",		  /* name */
			  false,				  /* partial_inplace */
			  0,					  /* src_mask */
			  0,					  /* dst_mask */
			  false),				  /* pcrel_offset */

		/* 30-bit PC relative jump.  */
		HOWTO(R_OC32_I_REL30,			/* type */
			  2,						/* rightshift: shift at very first */
			  4,						/* size */
			  30,						/* bitsize */
			  true,						/* pc_relative */
			  0,						/* bitpos */
			  complain_overflow_signed, /* complain_on_overflow: check after bit shift, thus bitsize is the real size after bit shift */
			  bfd_elf_generic_reloc,	/* special_function */
			  "R_OC32_I_REL30",			/* name */
			  false,					/* partial_inplace */
			  0,						/* src_mask for instruction code */
			  0x0fffffff,				/* dst_mask for instruction code */
			  true),					/* pcrel_offset */

		/* 20-bit conditional jump.  */
		HOWTO(R_OC32_I_CON20,			/* type */
			  2,						/* rightshift */
			  4,						/* size */
			  20,						/* bitsize */
			  true,						/* pc_relative */
			  5,						/* bitpos */
			  complain_overflow_signed, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	/* special_function */
			  "R_OC32_I_CON20",			/* name */
			  false,					/* partial_inplace */
			  0,						/* src_mask */
			  (0x0003ffff << 5),		/* dst_mask */
			  true),					/* pcrel_offset */

		/* 16-bit immediate.  */
		HOWTO(R_OC32_D_ABSLO,			  /* type */
			  0,						  /* rightshift */
			  4,						  /* size */
			  32,						  /* bitsize */
			  false,					  /* pc_relative */
			  10,						  /* bitpos */
			  complain_overflow_unsigned, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	  /* special_function */
			  "R_OC32_D_ABSLO",			  /* name */
			  false,					  /* partial_inplace */
			  0,						  /* src_mask */
			  (0x0000ffff << 10),		  /* dst_mask */
			  false),					  /* pcrel_offset */

		/* 16-bit immediate.  */
		HOWTO(R_OC32_D_ABSHI,			  /* type */
			  16,						  /* rightshift */
			  4,						  /* size */
			  32,						  /* bitsize */
			  false,					  /* pc_relative */
			  5,						  /* bitpos */
			  complain_overflow_unsigned, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	  /* special_function */
			  "R_OC32_D_ABSHI",			  /* name */
			  false,					  /* partial_inplace */
			  0,						  /* src_mask */
			  (0x0000ffff << 5),		  /* dst_mask */
			  false),					  /* pcrel_offset */

		/* 16-bit immediate.  */
		HOWTO(R_OC32_D_ABSHA,			  /* type */
			  16,						  /* rightshift */
			  4,						  /* size */
			  32,						  /* bitsize */
			  false,					  /* pc_relative */
			  5,						  /* bitpos */
			  complain_overflow_unsigned, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	  /* special_function */
			  "R_OC32_D_ABSHA",			  /* name */
			  false,					  /* partial_inplace */
			  0,						  /* src_mask */
			  (0x0000ffff << 5),		  /* dst_mask */
			  false),					  /* pcrel_offset */

		/* 16-bit immediate.  */
		HOWTO(R_OC32_D_GOTLO,			  /* type */
			  0,						  /* rightshift */
			  4,						  /* size */
			  32,						  /* bitsize */
			  true,					      /* pc_relative */
			  10,						  /* bitpos */
			  complain_overflow_unsigned, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	  /* special_function */
			  "R_OC32_D_GOTLO",			  /* name */
			  false,					  /* partial_inplace */
			  0,						  /* src_mask */
			  (0x0000ffff << 10),		  /* dst_mask */
			  false),					  /* pcrel_offset */

		/* 16-bit immediate.  */
		HOWTO(R_OC32_D_GOTHI,			  /* type */
			  16,						  /* rightshift */
			  4,						  /* size */
			  32,						  /* bitsize */
			  true,					  	  /* pc_relative */
			  5,						  /* bitpos */
			  complain_overflow_unsigned, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	  /* special_function */
			  "R_OC32_D_GOTHI",			  /* name */
			  false,					  /* partial_inplace */
			  0,						  /* src_mask */
			  (0x0000ffff << 5),		  /* dst_mask */
			  false),					  /* pcrel_offset */

		HOWTO(R_OC32_32,
			  0,						  /* rightshift */
			  4,						  /* size */
			  32,						  /* bitsize */
			  false,					  /* pc_relative */
			  0,						  /* bitpos */
			  complain_overflow_unsigned, /* complain_on_overflow */
			  bfd_elf_generic_reloc,	  /* special_function */
			  "R_OC32_32",				  /* name */
			  false,					  /* partial_inplace */
			  0,						  /* src_mask */
			  0xffffffff,				  /* dst_mask */
			  false),					  /* pcrel_offset */

};

/* Map BFD reloc types to OC32 ELF reloc types.  */

struct oc32_reloc_map
{
	bfd_reloc_code_real_type bfd_reloc_val;
	unsigned int oc32_reloc_val;
};

static const struct oc32_reloc_map oc32_reloc_map[] =
	{
		{BFD_RELOC_NONE, R_OC32_NONE},
		{BFD_RELOC_OC32_I_REL30, R_OC32_I_REL30},
		{BFD_RELOC_OC32_I_CON20, R_OC32_I_CON20},
		{BFD_RELOC_OC32_D_ABSLO, R_OC32_D_ABSLO},
		{BFD_RELOC_OC32_D_ABSHI, R_OC32_D_ABSHI},
		{BFD_RELOC_OC32_D_ABSHA, R_OC32_D_ABSHA},
		{BFD_RELOC_OC32_D_GOTLO, R_OC32_D_GOTLO},
		{BFD_RELOC_OC32_D_GOTHI, R_OC32_D_GOTHI},
		{BFD_RELOC_32, R_OC32_32},
};

/* Set the howto pointer for an OC32 ELF reloc.  */

static bool
oc32_info_to_howto_rela(bfd *abfd,
						arelent *cache_ptr,
						Elf_Internal_Rela *dst)
{
	unsigned int r_type;

	r_type = ELF32_R_TYPE(dst->r_info);
	if (r_type >= (unsigned int)R_OC32_max)
	{
		/* xgettext:c-format */
		_bfd_error_handler(_("%pB: unsupported relocation type %#x"),
						   abfd, r_type);
		bfd_set_error(bfd_error_bad_value);
		return false;
	}

	cache_ptr->howto = &oc32_elf_howto_table[r_type];
	return cache_ptr->howto != NULL;
}

/* Relocate an OC32 ELF section.  */

static int
oc32_elf_relocate_section(bfd *output_bfd,
						  struct bfd_link_info *info,
						  bfd *input_bfd,
						  asection *input_section,
						  bfd_byte *contents,
						  Elf_Internal_Rela *relocs,
						  Elf_Internal_Sym *local_syms,
						  asection **local_sections)
{
	Elf_Internal_Shdr *symtab_hdr;
	struct elf_link_hash_entry **sym_hashes;
	Elf_Internal_Rela *rel;
	Elf_Internal_Rela *relend;

	symtab_hdr = &elf_tdata(input_bfd)->symtab_hdr;
	sym_hashes = elf_sym_hashes(input_bfd);
	relend = relocs + input_section->reloc_count;

	for (rel = relocs; rel < relend; rel++)
	{
		reloc_howto_type *howto;
		unsigned long r_symndx;
		Elf_Internal_Sym *sym;
		asection *sec;
		struct elf_link_hash_entry *h;
		bfd_vma relocation;
		bfd_reloc_status_type r;
		const char *name;
		int r_type;

		r_type = ELF32_R_TYPE(rel->r_info);
		r_symndx = ELF32_R_SYM(rel->r_info);
		howto = oc32_elf_howto_table + r_type;
		h = NULL;
		sym = NULL;
		sec = NULL;

		if (r_symndx < symtab_hdr->sh_info)
		{
			sym = local_syms + r_symndx;
			sec = local_sections[r_symndx];
			relocation = _bfd_elf_rela_local_sym(output_bfd, sym, &sec, rel);

			name = bfd_elf_string_from_elf_section(input_bfd, symtab_hdr->sh_link, sym->st_name);
			name = name == NULL ? bfd_section_name(sec) : name;
		}
		else
		{
			bool unresolved_reloc, warned, ignored;

			RELOC_FOR_GLOBAL_SYMBOL(info, input_bfd, input_section, rel,
									r_symndx, symtab_hdr, sym_hashes,
									h, sec, relocation,
									unresolved_reloc, warned, ignored);

			name = h->root.root.string;
		}

		if (sec != NULL && discarded_section(sec))
			RELOC_AGAINST_DISCARDED_SECTION(info, input_bfd, input_section,
											rel, 1, relend, R_OC32_NONE, howto, 0, contents);

		if (bfd_link_relocatable(info))
			continue;

		/* Handle R_OC32_32 relocation specially */
		r = bfd_reloc_ok;
		if (r_type == R_OC32_32)
		{
			/* r_symndx will be STN_UNDEF (zero) only for relocs against symbols
			   from removed linkonce sections, or sections discarded by
			   a linker script.  */
			if (r_symndx == STN_UNDEF || (input_section->flags & SEC_ALLOC) == 0)
				continue;

			/* For non-PIC builds, handle dynamic symbols */
			if (h != NULL && h->dynindx != -1 && !h->non_got_ref && ((h->def_dynamic && !h->def_regular) || h->root.type == bfd_link_hash_undefweak || h->root.type == bfd_link_hash_undefined))
			{
				asection *sreloc = elf_section_data(input_section)->sreloc;
				if (sreloc != NULL)
				{
					Elf_Internal_Rela outrel;
					bfd_byte *loc;
					bool skip;

					skip = false;

					outrel.r_offset = _bfd_elf_section_offset(output_bfd, info, input_section, rel->r_offset);
					if (outrel.r_offset == (bfd_vma)-1)
						skip = true;
					else if (outrel.r_offset == (bfd_vma)-2)
						skip = true;
					outrel.r_offset += (input_section->output_section->vma + input_section->output_offset);

					if (skip)
						memset(&outrel, 0, sizeof outrel);
					else if (SYMBOL_REFERENCES_LOCAL(info, h))
					{
						outrel.r_info = ELF32_R_INFO(0, R_OC32_32);
						outrel.r_addend = relocation + rel->r_addend;
					}
					else
					{
						BFD_ASSERT(h->dynindx != -1);
						outrel.r_info = ELF32_R_INFO(h->dynindx, r_type);
						outrel.r_addend = rel->r_addend;
					}

					loc = sreloc->contents;
					loc += sreloc->reloc_count++ * sizeof(Elf32_External_Rela);
					bfd_elf32_swap_reloca_out(output_bfd, &outrel, loc);
				}
			}
			else
			{
				/* For local symbols, directly relocate */
				r = _bfd_final_link_relocate(howto, input_bfd, input_section,
											 contents, rel->r_offset,
											 relocation, rel->r_addend);
			}
		}
		else
		{
			/* Handle other relocation types normally */
			if (r_type == R_OC32_D_ABSHA)
			{
				r = _bfd_final_link_relocate(howto, input_bfd, input_section,
											 contents, rel->r_offset,
											 relocation, (rel->r_addend + 0x8000));
			}
			else
			{
				r = _bfd_final_link_relocate(howto, input_bfd, input_section,
											 contents, rel->r_offset,
											 relocation, rel->r_addend);
			}
		}

		if (r != bfd_reloc_ok)
		{
			const char *msg = NULL;

			switch (r)
			{
			case bfd_reloc_overflow:
				(*info->callbacks->reloc_overflow)(info, (h ? &h->root : NULL), name, howto->name,
												   (bfd_vma)0, input_bfd, input_section, rel->r_offset);
				break;

			case bfd_reloc_undefined:
				(*info->callbacks->undefined_symbol)(info, name, input_bfd, input_section, rel->r_offset, true);
				break;

			case bfd_reloc_outofrange:
				msg = _("internal error: out of range error");
				break;

			case bfd_reloc_notsupported:
				msg = _("internal error: unsupported relocation error");
				break;

			case bfd_reloc_dangerous:
				msg = _("internal error: dangerous relocation");
				break;

			default:
				msg = _("internal error: unknown error");
				break;
			}

			if (msg)
				(*info->callbacks->warning)(info, msg, name, input_bfd,
											input_section, rel->r_offset);
		}
	}

	return true;
}

/* Look up a BFD reloc code and return a HOWTO.  */

static reloc_howto_type *
oc32_reloc_type_lookup(bfd *abfd ATTRIBUTE_UNUSED, bfd_reloc_code_real_type code)
{
	unsigned int i;

	for (i = 0; i < sizeof(oc32_reloc_map) / sizeof(oc32_reloc_map[0]); i++)
		if (oc32_reloc_map[i].bfd_reloc_val == code)
			return &oc32_elf_howto_table[oc32_reloc_map[i].oc32_reloc_val];

	return NULL;
}

/* Look up a reloc name and return a HOWTO.  */

static reloc_howto_type *
oc32_reloc_name_lookup(bfd *abfd ATTRIBUTE_UNUSED, const char *r_name)
{
	unsigned int i;

	for (i = 0;
		 i < sizeof(oc32_elf_howto_table) / sizeof(oc32_elf_howto_table[0]);
		 i++)
		if (oc32_elf_howto_table[i].name != NULL && strcasecmp(oc32_elf_howto_table[i].name, r_name) == 0)
			return &oc32_elf_howto_table[i];

	return NULL;
}

#define ELF_ARCH bfd_arch_oc32
#define ELF_MACHINE_CODE EM_OC32
#define ELF_MAXPAGESIZE 0x1

#define TARGET_LITTLE_SYM oc32_elf32_vec
#define TARGET_LITTLE_NAME "elf32-oc32"

#define elf_info_to_howto_rel NULL
#define elf_info_to_howto oc32_info_to_howto_rela
#define elf_backend_relocate_section oc32_elf_relocate_section
#define elf_backend_can_gc_sections 1
#define elf_backend_rela_normal 1
#define bfd_elf32_bfd_reloc_type_lookup oc32_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup oc32_reloc_name_lookup

#include "elf32-target.h"
