script {
  links = {
    {
      1,
      1,
      5,
      1,
    },
    {
      2,
      1,
      1,
      1,
    },
    {
      3,
      1,
      2,
      1,
    },
    {
      4,
      1,
      2,
      2,
    },
  },
  nodes = {
    {
      name = 'components/propcomponent',
    },
    {
      name = 'vector2',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
  },
}