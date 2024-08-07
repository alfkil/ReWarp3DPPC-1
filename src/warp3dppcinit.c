
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/types.h>
#include <utility/tagitem.h>
#include <proto/warp3dppc.h>
#include <proto/warp3d.h>
#include <proto/graphics.h>
#include <stdarg.h>

struct Library *Warp3DBase;
struct Library *DOSBase;
struct Library *NewlibBase;
struct Library *GraphicsBase;
struct Warp3DIFace *IWarp3D;
struct DOSIFace *IDOS;
struct Interface *INewlib;
struct GraphicsIFace *IGraphics;

#if defined(DEBUG) || defined(PATCH_RADEONHD)
struct ExecIFace *iexec = 0;
#endif

ULONG PatchFlag = 0;	/* which patches should be enabled */

#include "LibHeader.h"
#include "warp3dppc.library_rev.h"
#include "warp3dppc_patch.h"

CONST UBYTE verstag[] = VERSTAG;

int32 _start(void);

int32 _start(void)
{
    return -1;
}

STATIC struct Library *libOpen(struct LibraryManagerInterface *Self, ULONG version)
{
    struct warp3dppcLibrary *LibBase = (struct warp3dppcLibrary *)Self->Data.LibBase; 

    if (version > VERSION)
    {
        return NULL;
    }

    LibBase->libNode.lib_OpenCnt++;
    return (struct Library *)LibBase;

}

STATIC APTR libClose(struct LibraryManagerInterface *Self)
{
    struct warp3dppcLibrary *LibBase = (struct warp3dppcLibrary *)Self->Data.LibBase;

    ((struct Library *)LibBase)->lib_OpenCnt--;

    return 0;
}

STATIC APTR libExpunge(struct LibraryManagerInterface *Self)
{
    APTR result = (APTR)0;
    struct warp3dppcLibrary *LibBase = (struct warp3dppcLibrary *)Self->Data.LibBase;
    if (LibBase->libNode.lib_OpenCnt == 0)
    {
	     result = (APTR)LibBase->segList;

        IExec->DropInterface(INewlib);
        IExec->CloseLibrary(NewlibBase);

        IExec->DropInterface((struct Interface *)IGraphics);
        IExec->CloseLibrary(GraphicsBase);

        IExec->DropInterface((struct Interface *)IDOS);
        IExec->CloseLibrary(DOSBase);

        IExec->DropInterface((struct Interface *)IWarp3D);
        IExec->CloseLibrary(Warp3DBase);

        IExec->Remove((struct Node *)LibBase);
        IExec->DeleteLibrary((struct Library *)LibBase);
    }
    else
    {
        result = (APTR)0;
        LibBase->libNode.lib_Flags |= LIBF_DELEXP;
    }
    return result;
}

STATIC struct Library *libInit(struct Library *LibraryBase, APTR seglist, struct Interface *ISys)
{
    struct warp3dppcLibrary *LibBase = (struct warp3dppcLibrary *)LibraryBase;

    IExec = (struct ExecIFace *)ISys;

#if defined(DEBUG) || defined(DPATCH_RADEONHD)
	iexec = IExec;
#endif

    LibBase->libNode.lib_Node.ln_Type = NT_LIBRARY;
    LibBase->libNode.lib_Node.ln_Pri  = 0;
    LibBase->libNode.lib_Node.ln_Name = "Warp3DPPC.library";
    LibBase->libNode.lib_Flags        = LIBF_SUMUSED|LIBF_CHANGED;
    LibBase->libNode.lib_Version      = VERSION;
    LibBase->libNode.lib_Revision     = REVISION;
    LibBase->libNode.lib_IdString     = VSTRING;

    LibBase->segList = (BPTR)seglist;


       NewlibBase = IExec->OpenLibrary("newlib.library", 0L);
       if (NewlibBase)
       {
           INewlib = IExec->GetInterface(NewlibBase, "main", 1, NULL);
           if (!INewlib)
               return NULL;
       } else return NULL;
       
       Warp3DBase = IExec->OpenLibrary("Warp3D.library", 50L);
       if (Warp3DBase)
       {
           IWarp3D = (struct Warp3DIFace *)IExec->GetInterface(Warp3DBase, "main", 1, NULL);
           if (!IWarp3D)
               return NULL;
       } else return NULL;

       GraphicsBase = IExec->OpenLibrary("graphics.library", 50L);
       if (GraphicsBase)
       {
           IGraphics = (struct GraphicsIFace *)IExec->GetInterface(GraphicsBase, "main", 1, NULL);
           if (!IGraphics)
               return NULL;
       } else return NULL;

       DOSBase = IExec->OpenLibrary("dos.library",50L);
       if (DOSBase)
       {
       		IDOS = (struct DOSIFace *)IExec->GetInterface(DOSBase, "main", 1, NULL);
		if (IDOS)
			{
			BPTR lock;
			lock = IDOS->Lock("ENV:Warp3DPPC/PatchAll", SHARED_LOCK);
			if (lock)
				{
				PatchFlag|=PATCH_ALL;
				IDOS->UnLock(lock);
				}
			lock = IDOS->Lock("ENV:Warp3DPPC/PatchDrawElements", SHARED_LOCK);
			if (lock)
				{
				PatchFlag|=PATCH_DRAWELEMENTS;
				IDOS->UnLock(lock);
				}
			lock = IDOS->Lock("ENV:Warp3DPPC/PatchDrawArray", SHARED_LOCK);
			if (lock)
				{
				PatchFlag|=PATCH_DRAWARRAY;
				IDOS->UnLock(lock);
				}
			lock = IDOS->Lock("ENV:Warp3DPPC/PatchDrawLine", SHARED_LOCK);
			if (lock)
				{
				PatchFlag|=PATCH_DRAWLINE;
				IDOS->UnLock(lock);
				}
			IExec->DropInterface((struct Interface *)IDOS);
			}
		IExec->CloseLibrary(DOSBase);
	}
       return (struct Library *)LibBase;
}

STATIC LONG _manager_Obtain(struct LibraryManagerInterface *Self)
{
    return Self->Data.RefCount++;
}

STATIC ULONG _manager_Release(struct LibraryManagerInterface *Self)
{
    return Self->Data.RefCount--;
}

/* Manager interface vectors */
STATIC CONST APTR lib_manager_vectors[] =
{
    (APTR)_manager_Obtain,
    (APTR)_manager_Release,
    NULL,
    NULL,
    (APTR)libOpen,
    (APTR)libClose,
    (APTR)libExpunge,
    NULL,
    (APTR)-1
};

STATIC CONST struct TagItem lib_managerTags[] =
{
    { MIT_Name,        (Tag)"__library"       },
    { MIT_VectorTable, (Tag)lib_manager_vectors },
    { MIT_Version,     1                        },
    { TAG_DONE,        0                        }
};

#include "warp3dppc_vectors.c"

extern APTR VecTable68K[];

STATIC CONST struct TagItem mainTags[] =
{
    { MIT_Name,        (Tag)"main" },
    { MIT_VectorTable, (Tag)main_vectors },
    { MIT_Version,     1 },
    { TAG_DONE,        0 }
};

STATIC CONST CONST_APTR libInterfaces[] =
{
    lib_managerTags,
    mainTags,
    NULL
};

STATIC CONST struct TagItem libCreateTags[] =
{
    { CLT_DataSize,    sizeof(struct warp3dppcLibrary) },
    { CLT_InitFunc,    (Tag)libInit },
    { CLT_Interfaces,  (Tag)libInterfaces},
    { CLT_Vector68K,   (Tag)VecTable68K },
    {TAG_DONE,         0 }
};

CONST struct Resident lib_res
#ifdef __GNUC__
__attribute__((used))
#endif
=
{
    RTC_MATCHWORD,
    (struct Resident *)&lib_res,
    (APTR)(&lib_res + 1),
    RTF_NATIVE|RTF_AUTOINIT,
    VERSION,
    NT_LIBRARY,
    0,
    "Warp3DPPC.library",
    VSTRING,
    (APTR)libCreateTags
};

